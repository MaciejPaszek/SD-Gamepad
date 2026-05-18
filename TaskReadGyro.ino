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

  AnalogMessage analogMessage;

  // Łańcuch znaków dla komunikatów o błędach
  char buffer[300];

  // Obiekt klasy MPU6050
  MPU6050 mpu6050 = MPU6050(SDA, SCL, MPU6050_ADR);

  // Ustawianie rejestrów zarządzania zasilaniem żyroskopu
  error = mpu6050.begin();
  
  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "mpu6050.begin():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
    Serial.print(buffer);
  }

  // Zakres pomiarowy żyroskopu
  // - MPU6050_RANGE_250_DEG_PER_SEC
  // - MPU6050_RANGE_500_DEG_PER_SEC
  // - MPU6050_RANGE_1000_DEG_PER_SEC
  // - MPU6050_RANGE_2000_DEG_PER_SEC
  error = mpu6050.setRange(MPU6050_RANGE_250_DEG_PER_SEC);

  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "mpu6050.setRange():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
    Serial.print(buffer);
  }

  // Licznik do kalibracji
  int i = 0;

  while(true)
  {
    // Pomiar prędkości z żyroskopu
    error = mpu6050.measure();

    if(error != MPU6050_NO_ERROR)
    {
      sprintf(buffer, "mpu6050.measure():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
      Serial.print(buffer);
    }
    else
    {
      // Resetowanie żyroskopu, gdy pomiary są zerowe
      if(mpu6050.gX == 0 && mpu6050.gY == 0 && mpu6050.gZ == 0)
      {
        // Ustawianie rejestrów zarządzania zasilaniem żyroskopu
        error = mpu6050.begin();
        
        if(error != MPU6050_NO_ERROR)
        {
          sprintf(buffer, "mpu6050.begin():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
          Serial.print(buffer);
        }

        // Zakres pomiarowy żyroskopu
        // - MPU6050_RANGE_250_DEG_PER_SEC
        // - MPU6050_RANGE_500_DEG_PER_SEC
        // - MPU6050_RANGE_1000_DEG_PER_SEC
        // - MPU6050_RANGE_2000_DEG_PER_SEC
        error = mpu6050.setRange(MPU6050_RANGE_250_DEG_PER_SEC);

        if(error != MPU6050_NO_ERROR)
        {
          sprintf(buffer, "mpu6050.setRange():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
          Serial.print(buffer);
        }
      }

      if(i < 100)
      {
        mpu6050.calibrate(i);
        i++;
      }

      mpu6050.calculate(0.1);

      // Wiadomość do kolejki o osi Y
      analogMessage.analogID = 0;
      analogMessage.analogVal = (int) ((mpu6050.posY - mpu6050.posMinY) / (mpu6050.posMaxY - mpu6050.posMinY) * 4095.0);

      if(xQueueSend( xQueueGyro, ( void * ) &analogMessage, 10 ) != pdTRUE)
      {
        Serial.println("xQueueGyro is full.");
      }

      // Wiadomość oś Z
      analogMessage.analogID = 1;
      analogMessage.analogVal = (int) ((mpu6050.posZ - mpu6050.posMinZ) / (mpu6050.posMaxZ - mpu6050.posMinZ) * 4095.0);

      if(xQueueSend( xQueueGyro, ( void * ) &analogMessage, 10 ) != pdTRUE)
      {
        Serial.println("xQueueGyro is full.");
      }

      // Wypisz
      sprintf(buffer, "gX:%d; gY:%d; gZ:%d;\t", mpu6050.gX, mpu6050.gY, mpu6050.gZ);
      Serial.print(buffer);

      //sprintf(buffer, "gAvgX:%d; gAvgY:%d; gAvgZ:%d;\t", mpu6050.gX, mpu6050.gY, mpu6050.gZ);
      //Serial.print(buffer);

      //sprintf(buffer, "vX:%f; vY:%f; vZ:%f;\t", mpu6050.vX, mpu6050.vY, mpu6050.vZ);
      //Serial.print(buffer);

      sprintf(buffer, "posX:%f; posY:%f; posZ:%f\n", mpu6050.posX, mpu6050.posY, mpu6050.posZ);
      Serial.print(buffer);

    }
    // Opóźnienie 100 ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
