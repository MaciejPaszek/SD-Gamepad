// https://github.com/lemmingDev/ESP32-BLE-Gamepad
// https://github.com/h2zero/NimBLE-Arduino
//#include <Arduino.h>
////#include <BleGamepad.h>

//BleGamepad bleGamepad("SD Gamepad", "Paszek i Suwart", 100);

//--------------------------------------------------
// Wejścia Cyfrowe
//--------------------------------------------------

#define NO_BUTTONS      14  // Liczba wejść cyfrowych

#define BUTTON_START    13  // Przycisk Start
#define BUTTON_SELECT   13  // Przycisk Wybierz
#define BUTTON_X         2  // Przycisk X
#define BUTTON_Y         0  // Przycisk Y
#define BUTTON_A         4  // Przycisk A
#define BUTTON_B        16  // Przycisk B
#define BUTTON_DPAD_L   13  // Przycisk kierunkowy lewy
#define BUTTON_DPAD_R   13  // Przycisk kierunkowy prawy
#define BUTTON_DPAD_U   13  // Przycisk kierunkowy górny
#define BUTTON_DPAD_D   13  // Przycisk kierunkowy dolny
#define BUTTON_LB       13  // Przycisk zderzak lewy
#define BUTTON_RB       13  // Przycisk zderzak prawy
#define BUTTON_LS       27  // Przycisk gałki analogowej lewej
#define BUTTON_RS       14  // Przycisk gałki analogowej prawej

//--------------------------------------------------
// Wejścia Analogowe
//--------------------------------------------------

#define NO_ANALOGS       6  // Liczba wejść analogowych

#define ANALOG_LX       34  // Gałka lewa oś X
#define ANALOG_LY       35  // Gałka lewa oś Y
#define ANALOG_RX       32  // Gałka prawa oś X
#define ANALOG_RY       33  // Gałka prawa oś Y
#define ANALOG_LT       25  // Spust lewy
#define ANALOG_LT       26  // Spust prawy

//--------------------------------------------------
// Żyroskop
//--------------------------------------------------

#define SCL             22  // Zegar I2C
#define SDA             23  // Dane I2C

//--------------------------------------------------
// Zadania
//--------------------------------------------------

// Kontroluj, które zadania należy utworzyć
#define CREATE_TASK_SERIAL                1
#define CREATE_TASK_BLUETOOTH             0
#define CREATE_TASK_READ_DIGITAL_INPUT    1
#define CREATE_TASK_READ_ANALOG_INPUT     1
#define CREATE_TASK_READ_GYRO             0

// Priorytety zadań
#define PRIORITY_TASK_SERIAL              2
#define PRIORITY_TASK_BLUETOOTH           1
#define PRIORITY_TASK_READ_DIGITAL_INPUT  3
#define PRIORITY_TASK_READ_ANALOG_INPUT   4
#define PRIORITY_TASK_READ_GYRO           5

//--------------------------------------------------
// Stałe statyczne
//--------------------------------------------------

// Statyczna tablica pinów przycisków
static const int BUTTONS[] = {
  BUTTON_START,
  BUTTON_SELECT,
  BUTTON_X,
  BUTTON_Y,
  BUTTON_A,
  BUTTON_B,
  BUTTON_DPAD_L,
  BUTTON_DPAD_R,
  BUTTON_DPAD_U,
  BUTTON_DPAD_D,
  BUTTON_LB,
  BUTTON_RB,
  BUTTON_LS,
  BUTTON_RS};

// Statyczna tablica pinów analogowych
static const int ANALOGS[] = {
  ANALOG_LX,
  ANALOG_LY,
  ANALOG_RX,
  ANALOG_RY,
  ANALOG_LT,
  ANALOG_LT};

//--------------------------------------------------
// Semafory
//--------------------------------------------------

SemaphoreHandle_t xSemaphore;

//--------------------------------------------------
// Kolejki
//--------------------------------------------------

static const uint8_t QueueLen = 30;
QueueHandle_t xQueue;

static const uint8_t QueueAnalogLen = 30;
QueueHandle_t xQueueAnalog;

//--------------------------------------------------
// Struktury
//--------------------------------------------------

typedef struct 
{
    int buttonID;
    bool buttonState;
}ButtonMessage;

typedef struct 
{
    int analogID;
    int analogVal;
}AnalogMessage;


//--------------------------------------------------
// Setup - punkt wejścia programu
//--------------------------------------------------

void setup()
{
  // Ustalenie prędkości transmisji szeregowej
  Serial.begin(115200);
  
  Serial.println("SD Gamepad startup.");

  // Bluetooth gamepad
  // BleGamepadConfiguration bleGamepadConfig;
  // bleGamepadConfig.setAutoReport(false); // This is true by default
  // bleGamepadConfig.setButtonCount(NO_BUTTONS);
  // bleGamepad.begin(&bleGamepadConfig);

  // Tryb wejść binarnych z rezystorem podciągającym do 3.3 V
  for(int i = 0; i < NO_BUTTONS; i++)
  {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
  
  // Tryb wejść analogowych
  for(int i = 0; i < NO_ANALOGS; i++)
  {
    pinMode(ANALOGS[i], INPUT);
  }

  xQueue = xQueueCreate( QueueLen, sizeof(ButtonMessage) );

  if( xQueue == NULL )
  {
    Serial.println("Queue could not be created with xQueueCreate");
  }

  xQueueAnalog = xQueueCreate( QueueAnalogLen, sizeof(AnalogMessage) );

  if( xQueueAnalog == NULL )
  {
    Serial.println("QueueAnalog could not be created with xQueueCreate");
  }

  // Zadanie 1 - Transmisja szeregowa
  if(CREATE_TASK_SERIAL)
  {
    xTaskCreate(
      TaskSerial,             // Funkcja realizowana przez zadanie
      "Task Serial",          // Nazwa zadania
      1024,                   // Rozmiar stosu zadania
      NULL,                   // Parametry funkcji
      PRIORITY_TASK_SERIAL,   // Priorytet zadania
      NULL);                  // Uchwyt do zadania
  }

  // Zadanie 1 - Transmisja szeregowa
  if(CREATE_TASK_BLUETOOTH)
  {
    xTaskCreate(
      TaskBluetooth,             // Funkcja realizowana przez zadanie
      "Task Bluetooth",          // Nazwa zadania
      1024,                   // Rozmiar stosu zadania
      NULL,                   // Parametry funkcji
      PRIORITY_TASK_BLUETOOTH,   // Priorytet zadania
      NULL);                  // Uchwyt do zadania
  }

  // Zadanie 2 - Odczyt wejść cyfrowych
  if(CREATE_TASK_READ_DIGITAL_INPUT)
  {
    xTaskCreate(
      TaskReadDigitalInput,
      "Task Read Digital Input",
      1024,
      NULL,
      PRIORITY_TASK_READ_DIGITAL_INPUT,
      NULL);
  }

  // Zadanie 3 - Odczyt wejść analogowych
  if(CREATE_TASK_READ_ANALOG_INPUT)
  {
    xTaskCreate(
      TaskReadAnalogInput,
      "Task Read Analog Input",
      1024,
      NULL,
      PRIORITY_TASK_READ_ANALOG_INPUT,
      NULL);
  }
}

void loop()
{
  //if (bleGamepad.isConnected())
  //{

  //}
}