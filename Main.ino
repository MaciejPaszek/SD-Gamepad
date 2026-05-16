// https://github.com/lemmingDev/ESP32-BLE-Gamepad
// https://github.com/h2zero/NimBLE-Arduino

#include <BleGamepad.h>
#include <Wire.h>

BleGamepad bleGamepad("SD Gamepad", "Paszek i Suwart", 100);

//--------------------------------------------------
// Wejścia Cyfrowe
//--------------------------------------------------

// Zmienić liczbę wejść cyfrowych z 14 do 10
#define NO_BUTTONS      14  // Liczba wejść cyfrowych

#define BUTTON_START    34  // Przycisk Start
#define BUTTON_SELECT    5  // Przycisk Wybierz
#define BUTTON_X        33  // Przycisk X
#define BUTTON_Y        35  // Przycisk Y
#define BUTTON_A        25  // Przycisk A
#define BUTTON_B        32  // Przycisk B
#define BUTTON_LB       18  // Przycisk zderzak lewy
#define BUTTON_RB       23  // Przycisk zderzak prawy
#define BUTTON_LS        2  // Przycisk gałki analogowej lewej
#define BUTTON_RS       27  // Przycisk gałki analogowej prawej

//--------------------------------------------------
// Wejścia Cyfrowe - Hatsune Switch (aka DPAD)
//--------------------------------------------------

#define BUTTON_DPAD_L   16  // Przycisk kierunkowy lewy
#define BUTTON_DPAD_R    4  // Przycisk kierunkowy prawy
#define BUTTON_DPAD_U   17  // Przycisk kierunkowy górny
#define BUTTON_DPAD_D    0  // Przycisk kierunkowy dolny

//--------------------------------------------------
// Wejścia Analogowe
//--------------------------------------------------

#define NO_ANALOGS       6  // Liczba wejść analogowych

#define ANALOG_LX       13  // Gałka lewa oś X  
#define ANALOG_LY       15  // Gałka lewa oś Y
#define ANALOG_RX       26  // Gałka prawa oś X
#define ANALOG_RY       14  // Gałka prawa oś Y
#define ANALOG_LT       36  // Spust lewy
#define ANALOG_RT       39  // Spust prawy

//--------------------------------------------------
// Żyroskop
//--------------------------------------------------

#define SDA             19  // Dane I2C
#define SCL             22  // Zegar I2C

//--------------------------------------------------
// Silnik
//--------------------------------------------------

#define MOT             12

//--------------------------------------------------
// Zadania
//--------------------------------------------------

// Kontroluj, które zadania należy utworzyć
#define CREATE_TASK_BLUETOOTH             1
#define CREATE_TASK_SERIAL                0
#define CREATE_TASK_READ_DIGITAL_INPUT    1
#define CREATE_TASK_READ_ANALOG_INPUT     1
#define CREATE_TASK_READ_GYRO             1
#define CREATE_TASK_MOTOR                 0

// Priorytety zadań
#define PRIORITY_TASK_BLUETOOTH           1
#define PRIORITY_TASK_SERIAL              2
#define PRIORITY_TASK_READ_DIGITAL_INPUT  3
#define PRIORITY_TASK_READ_ANALOG_INPUT   4
#define PRIORITY_TASK_READ_GYRO           5
#define PRIORITY_TASK_MOTOR               6

//
#define TEST_DIGITAL_INPUT 1


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

static String BUTTONS_NAMES[] = {
  "BUTTON_START",
  "BUTTON_SELECT",
  "BUTTON_X",
  "BUTTON_Y",
  "BUTTON_A",
  "BUTTON_B",
  "BUTTON_DPAD_L",
  "BUTTON_DPAD_R",
  "BUTTON_DPAD_U",
  "BUTTON_DPAD_D",
  "BUTTON_LB",
  "BUTTON_RB",
  "BUTTON_LS",
  "BUTTON_RS"};

// Statyczna tablica pinów analogowych
static const int ANALOGS[] = {
  ANALOG_LX,
  ANALOG_LY,
  ANALOG_RX,
  ANALOG_RY,
  ANALOG_LT,
  ANALOG_RT};

//--------------------------------------------------
// Semafory
//--------------------------------------------------

SemaphoreHandle_t xSemaphore;

//--------------------------------------------------
// Kolejki
//--------------------------------------------------

static const uint8_t QueueDigitalLen = 30;
QueueHandle_t xQueueDigital;

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
  
  // Konfiguracja gamepada
  BleGamepadConfiguration bleGamepadConfig;

  // Manualnie aktualizuj stan gamepada
  bleGamepadConfig.setAutoReport(false);

  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
  bleGamepadConfig.setButtonCount(NO_BUTTONS);
  bleGamepadConfig.setHatSwitchCount(1);
  //bleGamepadConfig.setVid(0xe502);
  //bleGamepadConfig.setPid(0xabcd);
  // Some non-Windows operating systems and web based gamepad testers don't like min axis set below 0, so 0 is set by default
  //bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  bleGamepadConfig.setAxesMin(0); // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  bleGamepadConfig.setAxesMax(4096); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal 

  bleGamepad.begin(&bleGamepadConfig);

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

  // Silnik
  pinMode(MOT, OUTPUT);


  // Kolejka wejść cyfrowych
  xQueueDigital = xQueueCreate( QueueDigitalLen, sizeof(ButtonMessage) );

  if( xQueueDigital == NULL )
  {
    Serial.println("Queue could not be created with xQueueCreate");
  }

  // Kolejka wejść analogowych
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
      4096,                   // Rozmiar stosu zadania
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
      4096,                   // Rozmiar stosu zadania
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

  if(CREATE_TASK_READ_GYRO)
  {
    xTaskCreate(
      TaskReadGyro,             // Funkcja realizowana przez zadanie
      "Task Read Gyro",          // Nazwa zadania
      4096,                   // Rozmiar stosu zadania
      NULL,                   // Parametry funkcji
      PRIORITY_TASK_READ_GYRO,   // Priorytet zadania
      NULL);                  // Uchwyt do zadania
  }

  if(CREATE_TASK_MOTOR)
  {
    xTaskCreate(
      TaskMotor,             // Funkcja realizowana przez zadanie
      "Task Motor",          // Nazwa zadania
      1024,                   // Rozmiar stosu zadania
      NULL,                   // Parametry funkcji
      PRIORITY_TASK_MOTOR,   // Priorytet zadania
      NULL);                  // Uchwyt do zadania
  }
}

void loop()
{
  // Do sprawdzenia
  if (bleGamepad.isConnected())
  {

  }
}