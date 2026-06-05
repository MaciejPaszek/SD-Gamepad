#include "Arduino.h"
#include "MPU6050.h"

// Konstruktor
MPU6050::MPU6050(uint8_t sda, uint8_t scl, int adr)
{
  // Adres I2C
  _adr = adr;

  // Roczpocznij komunikację I2C na określonych pinach
  Wire.begin(sda, scl);
}

byte MPU6050::begin()
{
  // Kod błędu
  byte error;

  // Ustaw rejestr PWR_MGMT_1
  Wire.beginTransmission(_adr);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00001001); // DEVICE_RESET = 0; SLEEP = 0; CYCLE = 0; 0; TEMP_DIS = 1; CLKSEL = 001 (X axis gyroscope);
  error = Wire.endTransmission();

  if(error != MPU6050_NO_ERROR)
  {
    return error;
  }

  // Ustaw rejestr PWR_MGMT_2
  Wire.beginTransmission(_adr);
  Wire.write(PWR_MGMT_2);
  Wire.write(0x00111000); // LP_WAKE_CTRL = 00; STBY_XA = 1; STBY_YA = 1; STBY_ZA = 1; STBY_XG = 0; STBY_YG = 0; STBY_ZG = 0;
  error = Wire.endTransmission();

  return error;
}

byte MPU6050::setRange(byte range)
{
  // Kod błędu
  byte error;

  // Bajt wpisywany do rejestru GYRO_CONFIG
  byte gyroConfig;

  switch(range)
  {
    case MPU6050_RANGE_250_DEG_PER_SEC:
      gyroConfig = 0b00000000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 00; 000;
      break;
    
    case MPU6050_RANGE_500_DEG_PER_SEC:
      gyroConfig = 0b00001000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 01; 000;
      break;

    case MPU6050_RANGE_1000_DEG_PER_SEC:
      gyroConfig = 0b00010000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 10; 000;
      break;

    case MPU6050_RANGE_2000_DEG_PER_SEC:
      gyroConfig = 0b00011000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 11; 000;
      break;

    default:
      return MPU6050_ERROR_INVAILD_ARGUMENT;
  }

  Wire.beginTransmission(_adr);
  Wire.write(GYRO_CONFIG);
  Wire.write(gyroConfig);
  error = Wire.endTransmission();
  
  // Jeśli sukces, to zapisz nowy stan
  if(error == MPU6050_NO_ERROR)
  {
    _range = range;
  }

  return error;
}

byte MPU6050::reset()
{
  // Kod błędu
  byte error;

  Wire.beginTransmission(_adr);
  Wire.write(PWR_MGMT_1);
  Wire.write(0b10000000); // DEVICE_RESET = 1; SLEEP = 0; CYCLE = 0; 0; TEMP_DIS = 1; CLKSEL = 000;
  error = Wire.endTransmission();

  return error;
}

byte MPU6050::measure()
{
  // Kod błędu
  byte error;

  // Ustaw adres odczytu na rejestr GYRO_XOUT_H
  Wire.beginTransmission(_adr);
  Wire.write(GYRO_XOUT_H);
  error = Wire.endTransmission();

  if(error != MPU6050_NO_ERROR)
  {
    return error;
  }

  // Zażądaj odczytu 6 bajtów danych rozpoczynając od adresu GYRO_XOUT_H
  Wire.requestFrom(_adr, 6);

  // Sprawdź, czy do odczytu jest co najmniej 6 bajtów
  if(Wire.available() < 6)
  {
    return MPU6050_ERROR_AVAILABLE;
  }

  // Odczytaj 6 bajtów danych
  gRaw[0] = Wire.read() << 8 | Wire.read(); 
  gRaw[1] = Wire.read() << 8 | Wire.read();
  gRaw[2] = Wire.read() << 8 | Wire.read();

  error = Wire.endTransmission();

  // Przekonwertuj int16 na int32
  for(int i = 0; i < 3; i++)
  {
    gRaw[i] = Int16ToInt32(gRaw[i]);
  }

  if(_calibration)
  {
    calibrate();
  }

  return error;
}

void MPU6050::zero()
{
  for(int i = 0; i < 3; i++)
  {
    pos[i] = 0;
    posAnalog[i] = 0;
  }
}

void MPU6050::offset()
{
  for(int i = 0; i < 3; i++)
  {
    gVal[i] = gRaw[i] - gOffset[i];
  }

  return;
}

void MPU6050::average()
{
  // Wpisz odczytaną wartość do tablicy
  for(int i = 0; i < 3; i++)
  {
    gPrev[i][_avgIndex] = gVal[i];
  }

  // Przesuń indeks zapisu do tablicy
  _avgIndex++;

  // Jeśli indeks zapisu do tablicy wykroczył poza tablicę, wróć na początek
  if(_avgIndex >= AVG_WINDOW_SIZE)
  {
    _avgIndex = 0;
  }

  // Wyzeruj poprzednie wartości średnie
  for(int i = 0; i < 3; i++)
  {
    gAvg[i] = 0;
  }

  // Zsumuj wszystkie wartości w oknie przesuwnym
  for(int j = 0; j < AVG_WINDOW_SIZE; j++)
  {
    for(int i = 0; i < 3; i++)
    {
      gAvg[i] += gPrev[i][j];
    }
  }

  // Podziel sumę wartości przez rozmiar okna przesuwnego
  for(int i = 0; i < 3; i++)
  {
    gAvg[i] = gAvg[i] / AVG_WINDOW_SIZE;
  }

  return;
}

void MPU6050::calibrationRequest()
{
  for(int i = 0; i < 3; i++)
  {
    _calibration[i] = true;
  }
}

void MPU6050::calibrate()
{
  // Wpisz odczytaną wartość do tablicy
  for(int i = 0; i < 3; i++)
  {
    gCalibration[i][_calibrationIndex] = gRaw[i];
  }

  // Przesuń indeks zapisu do tablicy
  _calibrationIndex++;

  // Jeśli indeks zapisu do tablicy wykroczył poza tablicę, wróć na początek
  if(_calibrationIndex >= CALIBRATION_WINDOW_SIZE)
  {
    _calibrationIndex = 0;

    for(int i = 0; i < 3; i++)
    {
      if(_calibration[i] == true)
      {
        int calibrationResult = 0;

        for(int j = 0; j < CALIBRATION_WINDOW_SIZE; j++)
        {
          calibrationResult += gCalibration[i][j];
        }

        calibrationResult = calibrationResult / CALIBRATION_WINDOW_SIZE;

        if(calibrationResult == _calibrationPrevResults[i])
        {
          Serial.print("Kalibracja zakończona na osi ");
          Serial.print(i);
          Serial.print(" z wynikiem ");
          Serial.println(calibrationResult);

          gOffset[i] = calibrationResult;
          _calibration[i] = false;
        }
        else
        {
          Serial.print("Kalibracja nie udana na osi ");
          Serial.print(i);
          Serial.print(" z wynikiem ");
          Serial.println(calibrationResult);
        }

        _calibrationPrevResults[i] = calibrationResult;
      }
    }
  }

  return;
}

void MPU6050::integrate(int h)
{
  // Całkowanie
  for(int i = 0; i < 3; i++)
  {
    // Całkowanie Stopni na milisekundę daje milistopnie
    pos[i] = pos[i] + gVal[i] * RANGE[_range] * h / 32768;
  }

  return;
}

void MPU6050::analog()
{
  for(int i = 0; i < 3; i++)
  {
    posAnalog[i] = analogMin + (analogMax - analogMin) * (pos[i] + 90000) / (180000);

    // // Ograniczenie dolne
    if(posAnalog[i] > analogMax)
    {
      posAnalog[i] = analogMax;
    }

    // Ograniczenie górne
    if(posAnalog[i] < analogMin)
    {
      posAnalog[i] = analogMin;
    }
  }

  return;
}
  
void MPU6050::csvLog()
{
  // Nagłówek pliku CSV
  if(_csvLineNumber == 0)
  {
    sprintf(buffer, "t; gX; gY; gZ; pX; pY; pZ");
    Serial.println(buffer);
  }

  if(_csvLineNumber <= 10 * 100)
  {
    // Linia pliku CSV
    sprintf(buffer, "%d; %d; %d; %d", _csvLineNumber,
    gVal[0], gVal[1], gVal[2], pos[0], pos[1], pos[2]);
    Serial.println(buffer);

    // Inkrementacja numeru linii
    _csvLineNumber++;
  }
}

int MPU6050::Int16ToInt32(int int16)
{
  if(int16 < 0b1000000000000000)
  {
    return int16;
  }
  
  return int16 - 0b10000000000000000;
}
