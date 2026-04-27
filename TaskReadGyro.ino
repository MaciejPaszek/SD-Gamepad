#include <mpu6050.h>
#include <stdio.h>

void TaskReadGyro(void *)
{
  char buffer[200];
  wakeSensor(0x68);
  float rawGX, rawGY, rawGZ; // initialise raw gyroscope variables
  float rawAX, rawAY, rawAZ; // initialise raw accelerometer variables

  Serial.println("Start TaskReadGyro()");
  while(true)
  {
    //Serial.println("TaskReadGyro()");
    readGyroData(0x68, rawGX, rawGY, rawGZ); // pass MPU6050 address and gyroscope values are written to 3 provided variables
    readAccelData(0x68, rawAX, rawAY, rawAZ); // pass MPU6050 address and accelerometer values are written to 3 provided variables

    sprintf(buffer, "%10.0f %10.0f %10.0f | %10.0f %10.0f %10.0f", rawGX, rawGY, rawGZ, rawAX, rawAY, rawAZ);
    Serial.println(buffer);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}