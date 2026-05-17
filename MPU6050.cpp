#include "Arduino.h"
#include "MPU6050.h"

// Konstruktor bezargumentowy
MPU6050::MPU6050()
{

}

byte MPU6050::begin(uint8_t sda, uint8_t scl, int adr)
{
  // Kod błędu
  byte error;

  // Adres I2C
  _adr = adr;

  // Roczpocznij I2C na określonych pinach
  Wire.begin(sda, scl);

  // Ustaw rejestr PWR_MGMT_1
  Wire.beginTransmission(_adr);
  Wire.write(PWR_MGMT_1);
  //Wire.write(0x00); // wakes sensor up
  Wire.write(0x00001001); // DEVICE_RESET = 0; SLEEP = 0; CYCLE = 0; 0; TEMP_DIS = 1; CLKSEL = 001 (X axis gyroscope);
  error = Wire.endTransmission();

  if(error != MPU6050_NO_ERROR)
  {
    return error;
  } 

  Wire.beginTransmission(_adr);
  Wire.write(PWR_MGMT_2);
  Wire.write(0x00111000); // LP_WAKE_CTRL = 00; STBY_XA = 1; STBY_YA = 1; STBY_ZA = 1; STBY_XG = 0; STBY_YG = 0; STBY_ZG = 0;
  error = Wire.endTransmission();

  // Zwracanie wartości błędu
  return error;
}

byte MPU6050::setRange(byte range)
{
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
  byte error = Wire.endTransmission();

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
  // Ustaw adres odczytu na rejestr GYRO_XOUT_H
  Wire.beginTransmission(_adr);
  Wire.write(GYRO_XOUT_H);
  // Zakończ transmisję bez resetowania
  uint8_t error = Wire.endTransmission(false);

  // Zażądaj odczytu 6 bajtów danych rozpoczynając od adresu GYRO_XOUT_H
  Wire.requestFrom(_adr, 6);

  // Sprawdź, czy do odczytu jest co najmniej 6 bajtów
  if(Wire.available() < 6)
  {
    return MPU6050_ERROR_AVAILABLE;
  }

  // Odczytaj 6 bajtów danych
  gX = Wire.read() << 8 | Wire.read(); 
  gY = Wire.read() << 8 | Wire.read();
  gZ = Wire.read() << 8 | Wire.read();

  // Przekonweryuj int16 na int32
  gX = Int16ToInt32(gX);
  gY = Int16ToInt32(gY);
  gZ = Int16ToInt32(gZ);

  vX = gX * _range / 32767.0;
  vY = gY * _range / 32767.0;
  vZ = gZ * _range / 32767.0;

  error = Wire.endTransmission();

  return error;
}

int MPU6050::Int16ToInt32(int int16)
{
  if(int16 < 0b1000000000000000)
  {
    return int16;
  }
  
  return int16 - 0b10000000000000000;
}
