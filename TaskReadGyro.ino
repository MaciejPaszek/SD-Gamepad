#include <stdio.h>
#include "MPU6050.h"

// Adres 
#define MPU6050_ADR 0x68

#define PWR_MGMT_1 0x6b
#define GYRO_XOUT_H 0x43

void TaskReadGyro(void *)
{
  // Kod błędu
  byte error;

  // Łańcuch znakó dla komunikatów o błędach
  char buffer[255];

  // Obiekt klasy MPU6050
  MPU6050 mpu6050 = MPU6050();

  // Otwieranie połączenia
  error = mpu6050.begin(SDA, SCL, MPU6050_ADR);
  
  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "begin error: 0x%02x\n", error);
    Serial.print(buffer);
  }

  // Ustalanie zakresu żyroskopu
  // - MPU6050_RANGE_250_DEG_PER_SEC
  // - MPU6050_RANGE_500_DEG_PER_SEC
  // - MPU6050_RANGE_1000_DEG_PER_SEC
  // - MPU6050_RANGE_2000_DEG_PER_SEC
  error = mpu6050.setRange(MPU6050_RANGE_250_DEG_PER_SEC);

  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "setRange error: 0x%02x\n", error);
    Serial.print(buffer);
  }

  // Zmienne na orientację żyroskopu (całka z prędkości)
  int X = 0;
  int Y = 0;
  int Z = 0;
  
  int eps = 1000;

  while(true)
  {
    // Wykonaj pomiar z żyroskopu
    mpu6050.measure();



    // Całka z prędkości strefa nieczułości
    if(mpu6050.gX < -eps || mpu6050.gX > eps)
    {
      X += mpu6050.gX;
    }

    if(mpu6050.gY < -eps || mpu6050.gY > eps)
    {
      Y += mpu6050.gY;
    }

    if(mpu6050.gZ < -eps || mpu6050.gZ > eps)
    {
      Z += mpu6050.gZ;
    }

 

    // Wypisz
    sprintf(buffer, "gX:%d;\tgY:%d;\tgZ:%d;\tX:%d;\tY:%d;\tZ:%d", mpu6050.gX, mpu6050.gY, mpu6050.gZ, X, Y, Z);
    Serial.println(buffer);

    // Opóźnienie 100 ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
