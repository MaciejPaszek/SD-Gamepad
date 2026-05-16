#include <stdio.h>
#include "MPU6050.h"

// Adres 
#define MPU6050_ADR 0x68

#define PWR_MGMT_1 0x6b
#define GYRO_XOUT_H 0x43

void TaskReadGyro(void *)
{
  byte error;
  char buffer[200];

  MPU6050 mpu6050 = MPU6050();

  error = mpu6050.begin(SDA, SCL, MPU6050_ADR);
  
  //if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "begin error: 0x%02x\n", error);
    Serial.print(buffer);
  }

  //mpu6050.setRange(MPU6050_RANGE_2000_DEG_PER_SEC);
  error = mpu6050.setRange(MPU6050_RANGE_2000_DEG_PER_SEC);

  //if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "setRange error: 0x%02x\n", error);
    Serial.print(buffer);
  }

  int X = 0;
  int Y = 0;
  int Z = 0;
  
  while(true)
  {
    // Wykonaj pomiar z żyroskopu
    mpu6050.measure();

    //gyroX = mpu6050.gX;
    //gyroY = mpu6050.gY;
    //gyroZ = mpu6050.gZ;

    if(mpu6050.gX == 0 & mpu6050.gY == 0 & mpu6050.gZ == 0)
    {
      //mpu6050.reset();
    }

    X += mpu6050.gX;
    Y += mpu6050.gY;
    Z += mpu6050.gZ;

    sprintf(buffer, "gX:%d;\tgY:%d;\tgZ:%d;\tX:%d;\tY:%d;\tZ:%d", mpu6050.gX, mpu6050.gY, mpu6050.gZ, X, Y, Z);
    Serial.println(buffer);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

bool ReadGyroscope(int &gyroX, int &gyroY, int &gyroZ)
{
  Wire.beginTransmission(MPU6050_ADR); 
  Wire.write(GYRO_XOUT_H); // used as starting address to read all other values
  uint8_t error = Wire.endTransmission(false); // stores success/error code in variable and keeps sensor active as we need to read data

  Wire.requestFrom(MPU6050_ADR, 6, true);

  gyroX = Wire.read() << 8 | Wire.read(); 
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  gyroX = Int16ToInt32(gyroX);
  gyroY = Int16ToInt32(gyroY);
  gyroZ = Int16ToInt32(gyroZ);

  return true;
}

int Int16ToInt32(int int16)
{
  if(int16 >= 0b1000000000000000)
  {
    return int16 - 0b10000000000000000;
  }

  return int16;
}
