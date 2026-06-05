#include <stdio.h>
#include "MPU6050.h"

// Adres 
#define MPU6050_ADR 0x68

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
  error = mpu6050.setRange(MPU6050_RANGE_2000_DEG_PER_SEC);

  if(error != MPU6050_NO_ERROR)
  {
    sprintf(buffer, "mpu6050.setRange():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
    Serial.print(buffer);
  }

  // Licznik do kalibracji
  int i = 0;

  static int RequestMessage = 0;

  while(true)
  {
    // Odczytaj kolejkę
    if( xQueueGyroConfig != NULL )
    {
      while( xQueueReceive( xQueueGyroConfig, (void *)&RequestMessage, 0 ) == pdPASS )
      {
        if(RequestMessage == MPU6050_CALIBRATION_REQUEST)
        {
          mpu6050.calibrationRequest();
          Serial.println("Rozpoczynam kalibrację...");
        }

        if(RequestMessage == MPU6050_ZERO_REQUEST)
        {
          mpu6050.zero();
          Serial.println("Wyzerowano");
        }
      }
    }

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
      if(mpu6050.gRaw[0] == 0 && mpu6050.gRaw[1] == 0 && mpu6050.gRaw[2] == 0)
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
        error = mpu6050.setRange(MPU6050_RANGE_2000_DEG_PER_SEC);

        if(error != MPU6050_NO_ERROR)
        {
          sprintf(buffer, "mpu6050.setRange():\t%s (0x%02x)\n", ERROR_DESC[error].c_str(), error);
          Serial.print(buffer);
        }
      }

      mpu6050.calibrate();
      mpu6050.offset();
      mpu6050.average();
      mpu6050.integrate(10);
      mpu6050.analog();
      //mpu6050.csvLog();

      //if(i < 10000)
      //{
        //mpu6050.calibrate(i);
        //i++;
      //}

      //Wiadomość do kolejki o osi Y
      analogMessage.analogID = 0;
      analogMessage.analogVal = mpu6050.posAnalog[1];

      if(xQueueSend( xQueueGyro, ( void * ) &analogMessage, 10 ) != pdTRUE)
      {
        Serial.println("xQueueGyro is full.");
      }

      //Wiadomość oś Z
      analogMessage.analogID = 1;
      analogMessage.analogVal = mpu6050.posAnalog[2];

      if(xQueueSend( xQueueGyro, ( void * ) &analogMessage, 10 ) != pdTRUE)
      {
       Serial.println("xQueueGyro is full.");
      }

      // // Wypisz
      // sprintf(buffer, "gRawY:%d; gValY:%d; gAvgY:%d; gCal:%d\n",
      //   mpu6050.gRaw[1], mpu6050.gVal[1], mpu6050.gAvg[1], 
      //   mpu6050._calibrationPrevResults[1]);
      // Serial.print(buffer);

      //sprintf(buffer, "gX:%d; gY:%d; gZ:%d; gAvgX:%d; gAvgY:%d; gAvgZ:%d\n",
      //        mpu6050.gX, mpu6050.gY,  mpu6050.gZ, mpu6050.gAvgX, mpu6050.gAvgY, mpu6050.gAvgZ);
      //Serial.print(buffer);
    }
    // Opóźnienie 10 ms
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
