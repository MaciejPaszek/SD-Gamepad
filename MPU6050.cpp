#include "Arduino.h"
#include "MPU6050.h"

// Konstruktor
MPU6050::MPU6050(uint8_t sda, uint8_t scl, int adr)
{
  // Adres I2C
  _adr = adr;

  // Roczpocznij I2C na określonych pinach
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

  // Bajt rejestr
  byte gyroConfig;

  switch(range)
  {
    case MPU6050_RANGE_250_DEG_PER_SEC:
      _range = 250.0;
      gyroConfig = 0b00000000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 00; 000;
      break;
    
    case MPU6050_RANGE_500_DEG_PER_SEC:
      _range = 500.0;
      gyroConfig = 0b00001000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 01; 000;
      break;

    case MPU6050_RANGE_1000_DEG_PER_SEC:
      _range = 1000.0;
      gyroConfig = 0b00010000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 10; 000;
      break;

    case MPU6050_RANGE_2000_DEG_PER_SEC:
      _range = 2000.0;
      gyroConfig = 0b00011000; // XG_ST = 0; YG_ST = 0; ZG_ST = 0; FS_SEL = 11; 000;
      break;

    default:
      return MPU6050_ERROR_INVAILD_ARGUMENT;
  }

  Wire.beginTransmission(_adr);
  Wire.write(GYRO_CONFIG);
  Wire.write(gyroConfig);
  error = Wire.endTransmission();

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
    //gX = 0.0;
    //gY = 0.0;
    //gZ = 0.0;

    //posX = 0.0;
    //posY = 0.0;
    //posZ = 0.0;

    return MPU6050_ERROR_AVAILABLE;
  }

  // Odczytaj 6 bajtów danych
  gX = Wire.read() << 8 | Wire.read(); 
  gY = Wire.read() << 8 | Wire.read();
  gZ = Wire.read() << 8 | Wire.read();

  error = Wire.endTransmission();

  // Przekonweryuj int16 na int32
  gX = Int16ToInt32(gX) - gOffsetX;
  gY = Int16ToInt32(gY) - gOffsetY;
  gZ = Int16ToInt32(gZ) - gOffsetZ;

  return error;
}

void MPU6050::average()
{
  gPrevX[avgIndex] = gX;
  gPrevY[avgIndex] = gY;
  gPrevZ[avgIndex] = gZ;

  avgIndex++;

  if(avgIndex >= AVG_WINDOW_SIZE)
  {
    avgIndex = 0;
  }

  gAvgX = 0;
  gAvgY = 0;
  gAvgZ = 0;

  for(int i = 0; i < AVG_WINDOW_SIZE; i++)
  {
    gAvgX += gPrevX[i];
    gAvgY += gPrevY[i];
    gAvgZ += gPrevZ[i];
  }

  gAvgX = gAvgX / AVG_WINDOW_SIZE;
  gAvgY = gAvgY / AVG_WINDOW_SIZE;
  gAvgZ = gAvgZ / AVG_WINDOW_SIZE;

}

void MPU6050::calibrate(int i)
{
  gOffsetX = (gOffsetX * i + gX) / (i + 1.0);
  gOffsetY = (gOffsetY * i + gY) / (i + 1.0);
  gOffsetZ = (gOffsetZ * i + gZ) / (i + 1.0);

  return;
}

void MPU6050::calculate(float h)
{
  // Oblicz prędkości w stopniach na sekundę
  vX = (gX - gAvgX) * _range / 32767.0;
  vY = (gY - gAvgY) * _range / 32767.0;
  vZ = (gZ - gAvgZ) * _range / 32767.0;

  if(vX < -eps || vX > eps)
  {
    // Wykonaj krok na osi X
    posX = posX + vX * h;
    
    // Ograniczenie na oś
    if(posX < posMinX) { posX = posMinX; }
    if(posX > posMaxX) { posX = posMaxX; }

    // Przelicz na slider
    analogX = analogMin + (int) ((posX - posMinX) / (posMaxX - posMinX) * (analogMax - analogMin));
  }

  if(vY < -eps || vY > eps)
  {
    // Wykonaj krok na osi Y
    posY = posY + vY * h;
  
    // Ograniczenie na oś Y
    if(posY < posMinY) { posY = posMinY; }
    if(posY > posMaxY) { posY = posMaxY; }

    // Przelicz na slider
    analogY = analogMin + (int) ((posY - posMinY) / (posMaxY - posMinY) * (analogMax - analogMin));
  }

  if(vZ < -eps || vZ > eps)
  {
    // Wykonaj krok na osi Z
    posZ = posZ + vZ * h; 

    // Ograniczenie na oś Z
    if(posZ < posMinZ) { posZ = posMinZ; }
    if(posZ > posMaxZ) { posZ = posMaxZ; }

    // Przelicz na slider
    analogZ = analogMin + (int) ((posZ - posMinZ) / (posMaxZ - posMinZ) * (analogMax - analogMin));
  }
  
  return;
}

int MPU6050::Int16ToInt32(int int16)
{
  if(int16 < 0b1000000000000000)
  {
    return int16;
  }
  
  return int16 - 0b10000000000000000;
}
