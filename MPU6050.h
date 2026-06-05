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

#define CALIBRATION_WINDOW_SIZE 100
#define AVG_WINDOW_SIZE 10

static int RANGE[] = {
   250,
   500,
  1000,
  2000,
};

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
    // Konstruktor
    MPU6050(uint8_t sda, uint8_t scl, int adr);
    byte begin();
    byte reset();
    byte setRange(byte range);
    byte measure();

    // Zerowanie
    void zero();

    // Obliczenia
    void offset();
    void calibrate();
    void calibrationRequest();
    void average();
    void integrate(int h);
    void analog();

    void csvLog();

    // Surowe odczyty z żyroskopu w zakresie od -32768 do 32767
    int gRaw[3] = {0};

    // Offset odczytów z żyroskopu
    int gOffset[3] = { -86 ,   6  ,  15};

    // Offset odczytów z żyroskopu
    int gCalibration[3][CALIBRATION_WINDOW_SIZE] = {0};

    // Odczyty z żyroskopu przesunięte o offset, w zakresie od -32768 - off do 32767 - off
    int gVal[3] = {0};

    // Okno przesuwne
    int gPrev[3][AVG_WINDOW_SIZE] = {0};

    // Wartości z żyroskopu filtrowane oknem przesuwnym, w zakresie od -32768 - off do 32767 - off
    int gAvg[3] = {0};

    // Okres pomiarów w milisekundach
    // int h = 100;

    // Strefa nieczułości [-eps, eps]
    int eps = 5;

    // Pozycja żyroskopu w milistopniach
    int pos[3] = {0};

    // Ograniczenia pozycji żyroskopu w milistopniach
    int posMin[3] = {-90000};

    // Ograniczenia pozycji żyroskopu w milistopniach
    int posMax[3] = { 90000};

    // Zakres osi analogowej
    int analogMin =    0;
    int analogMax = 4095;

    // Pozycja żyroskopu skalowana do osi analogowej
    int posAnalog[3] = {0};
    int _calibrationPrevResults[3] = {0};
    bool _calibration[3] = {false};

  private:

    // Indeks okna przesuwnego
    int _avgIndex = 0;
    int _calibrationIndex = 0;
    int _adr;
    int _range;
    int Int16ToInt32(int int16);
    int _csvLineNumber = 0;
    char buffer[255] = {' '};
};

#endif