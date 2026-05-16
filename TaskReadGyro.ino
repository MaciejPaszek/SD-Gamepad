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
  error = mpu6050.setRange(MPU6050_RANGE_2000_DEG_PER_SEC);

  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "setRange error: 0x%02x\n", error);
    Serial.print(buffer);
  }

  // Zmienne na orientację żyroskopu (całka z prędkości)
  
  // Pozycja początkowa
  float X = 0.0;
  float Y = 0.0;
  float Z = 0.0;
  
  // Granica strefy nieczułości 5 stopni na sekundę
  float eps = 5.0;

  while(true)
  {
    // Wykonaj pomiar z żyroskopu
    mpu6050.measure();

    // Całka z prędkości strefa nieczułości
    if(mpu6050.vX < -eps || mpu6050.vX > eps)
    {
      X += mpu6050.vX * 0.1; // Uwzględniamy h = 100 ms
    }

    if(mpu6050.vY < -eps || mpu6050.vY > eps)
    {
      Y += mpu6050.vY * 0.1;
    }

    if(mpu6050.vZ < -eps || mpu6050.vZ > eps)
    {
      Z += mpu6050.vZ * 0.1;
    }

    // Wypisz
    sprintf(buffer, "gX:%d; gY:%d; gZ:%d;\t vX:%f; vY:%f; vZ:%f;\t X:%f; Y:%f; Z:%f\n",
            mpu6050.gX, mpu6050.gY, mpu6050.gZ, mpu6050.vX, mpu6050.vY, mpu6050.vZ, X, Y, Z);
    Serial.println(buffer);

    // Opóźnienie 100 ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
