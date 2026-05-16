//#include <mpu6050.h>
#include <stdio.h>

#define MPU6050_ADR 0x68

#define PWR_MGMT_1 0x6b
#define GYRO_XOUT_H 0x43

void TaskReadGyro(void *)
{
  char buffer[200];



  int gyroX, gyroY, gyroZ; // initialise raw gyroscope variables
  int gyroPrev = 0;
  int X = 0;
  int Y = 0;
  int Z = 0;

  //////////
  // Wake UP
  //////////

  Wire.begin(SDA, SCL);
  Wire.beginTransmission(MPU6050_ADR);
  Wire.write(PWR_MGMT_1); // global power management register
  Wire.write(0x00); // wakes sensor up
  uint8_t error =  Wire.endTransmission(); // returns success/error code

  //////////
  // Wake UP
  //////////

  Serial.println("Start TaskReadGyro()");
  while(true)
  {
    //Serial.println("TaskReadGyro()");
    
    ReadGyroscope(gyroX, gyroY, gyroZ);

    // Wire.beginTransmission(MPU6050_ADR); 
    // Wire.write(GYRO_XOUT_H);
    // uint8_t error = Wire.endTransmission(false);
    // Wire.requestFrom(MPU6050_ADR, 6, true);
    
    // gyroX = Wire.read() << 8 | Wire.read(); 
    // gyroY = Wire.read() << 8 | Wire.read();
    // gyroZ = Wire.read() << 8 | Wire.read();

    // gyroX = Int16ToInt32(gyroX);
    // gyroY = Int16ToInt32(gyroY);
    // gyroZ = Int16ToInt32(gyroZ);

    X += gyroX;
    Y += gyroY;
    Z += gyroZ;

    sprintf(buffer, "gX:%d; gY:%d; gZ:%d; X:%d; Y:%d; Z:%d", gyroX, gyroY, gyroZ, X, Y, Z);
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
