/*
  MPU6050.h
  (c) Maciej Paszek 2026
*/

#ifndef MPU6050_h
#define MPU6050_h

#include "MPU6050.h"
#include <Wire.h>

// Rejestry konfiguracyjne
#define CONFIG 0x1a
#define GYRO_CONFIG 0x1b

// Rejestry danych pomiarowych z żyroskopu
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

// Rejestry zarządzania zasilaniem
#define PWR_MGMT_1 0x6b
#define PWR_MGMT_2 0x6c

#define MPU6050_NO_ERROR            0
#define MPU6050_ERROR_DATA_TOO_LONG 1
#define MPU6050_ERROR_NACK_ADRESS   2
#define MPU6050_ERROR_NACK_DATA     3
#define MPU6050_ERROR_OTHER         4
#define MPU6050_ERROR_TIMEOUT       5
#define MPU6050_ERROR_AVAILABLE     6
#define MPU6050_ERROR_INVAILD_ARGUMENT 7

#define MPU6050_RANGE_250_DEG_PER_SEC  0
#define MPU6050_RANGE_500_DEG_PER_SEC  1
#define MPU6050_RANGE_1000_DEG_PER_SEC 2
#define MPU6050_RANGE_2000_DEG_PER_SEC 3

#define AVG_WINDOW_SIZE 10

static const String ERROR_DESC[] = {
  "MPU6050_NO_ERROR",
  "MPU6050_ERROR_DATA_TOO_LONG",
  "MPU6050_ERROR_NACK_ADRESS",
  "MPU6050_ERROR_NACK_DATA",
  "MPU6050_ERROR_OTHER",
  "MPU6050_ERROR_TIMEOUT",
  "MPU6050_ERROR_AVAILABLE",
  "MPU6050_ERROR_INVAILD_ARGUMENT"
};

class MPU6050
{
  public:
    MPU6050(uint8_t sda, uint8_t scl, int adr);
    byte begin();
    byte reset();
    byte setRange(byte range);
    byte measure();
    //byte zero();
    void calibrate(int i);
    void average();
    void calculate(float h);

    // Odczyt żyroskopu na osi X w zakresie od -32768 do 32767
    int gX;
    // Odczyt żyroskopu na osi Y w zakresie od -32768 do 32767
    int gY;
    // Odczyt żyroskopu na osi Z w zakresie od -32768 do 32767
    int gZ;

    int avgIndex = 0;
    int gPrevX[AVG_WINDOW_SIZE] = {0};
    int gPrevY[AVG_WINDOW_SIZE] = {0};
    int gPrevZ[AVG_WINDOW_SIZE] = {0};

    // Odczyt średni do kalibracji
    int gOffsetX = 26;
    int gOffsetY = 59;
    int gOffsetZ =  9;

    // Odczyt średni na wyjście
    int gAvgX = 0.0;
    int gAvgY = 0.0;
    int gAvgZ = 0.0;

    // Odczyt żyroskopu na osi X w stopniach na sekundę
    float vX;
    // Odczyt żyroskopu na osi Y w stopniach na sekundę
    float vY;
    // Odczyt żyroskopu na osi Z w stopniach na sekundę
    float vZ;

    // Strefa nieczułości [-eps, eps] w stopniach
    float eps = 5.0;

    // Pozycja żyroskopu na osi X w stopniach
    float posX = 0.0;
    // Pozycja żyroskopu na osi Y w stopniach
    float posY = 0.0;
    // Pozycja żyroskopu na osi Z w stopniach
    float posZ = 0.0;

    // Minimalna pozycja żyroskopu na osi X w stopniach
    float posMinX = -90.0;
    // Pozycja żyroskopu na osi Y w stopniach
    float posMinY = -90.0;
    // Pozycja żyroskopu na osi Z w stopniach
    float posMinZ = -90.0;

    // Pozycja żyroskopu na osi X w stopniach
    float posMaxX =  90.0;
    // Pozycja żyroskopu na osi Y w stopniach
    float posMaxY =  90.0;
    // Pozycja żyroskopu na osi Z w stopniach
    float posMaxZ =  90.0;

    // Zakres slidera
    int analogMin =    0;
    int analogMax = 4095;

    // Pozycja żyroskopu na osi X w zakresie od 0 do 4095
    int analogX = 0;
    // Pozycja żyroskopu na osi Y w zakresie od 0 do 4095
    int analogY = 0;
    // Pozycja żyroskopu na osi Z w zakresie od 0 do 4095
    int analogZ = 0;


  private:
    int _adr;
    float _range = 250.0;
    int Int16ToInt32(int int16);
};

#endif