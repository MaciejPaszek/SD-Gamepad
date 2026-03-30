#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//--------------------------------------------------
// Wejścia Cyfrowe
//--------------------------------------------------

#define NO_BUTTONS      14  // Liczba wejść cyfrowych

#define BUTTON_START     2  // Przycisk Start
#define BUTTON_SELECT    0  // Przycisk Wybierz
#define BUTTON_X         4  // Przycisk X
#define BUTTON_Y        16  // Przycisk Y
#define BUTTON_A        13  // Przycisk A
#define BUTTON_B        13  // Przycisk B
#define BUTTON_DPAD_L   13  // Przycisk kierunkowy lewy
#define BUTTON_DPAD_R   13  // Przycisk kierunkowy prawy
#define BUTTON_DPAD_U   13  // Przycisk kierunkowy górny
#define BUTTON_DPAD_D   13  // Przycisk kierunkowy dolny
#define BUTTON_LB       13  // Przycisk zderzak lewy
#define BUTTON_RB       13  // Przycisk zderzak prawy
#define BUTTON_LS       13  // Przycisk gałki analogowej lewej
#define BUTTON_RS       13  // Przycisk gałki analogowej prawej

//--------------------------------------------------
// Wejścia Analogowe
//--------------------------------------------------

#define NO_ANALOGS       6  // Liczba wejść analogowych

#define ANALOG_LX       17  // Gałka lewa oś X
#define ANALOG_LY       15  // Gałka lewa oś Y
#define ANALOG_RX       15  // Gałka prawa oś X
#define ANALOG_RY       15  // Gałka prawa oś Y
#define ANALOG_LT       15  // Spust lewy
#define ANALOG_LT       15  // Spust prawy

//--------------------------------------------------
// Żyroskop
//--------------------------------------------------

#define SCL             22  // Zegar I2C
#define SDA             23  // Dane I2C

//--------------------------------------------------
// Zadania
//--------------------------------------------------

#define CREATE_TASK_SERIAL                1
#define CREATE_TASK_READ_DIGITAL_INPUT    1
#define CREATE_TASK_READ_ANALOG_INPUT     0
#define CREATE_TASK_READ_GYRO             0

#define PRIORITY_TASK_SERIAL              1
#define PRIORITY_TASK_READ_DIGITAL_INPUT  2
#define PRIORITY_TASK_READ_ANALOG_INPUT   3
#define PRIORITY_TASK_READ_GYRO           4

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

//--------------------------------------------------
// Setup - punkt wejścia programu
//--------------------------------------------------

typedef struct 
{
    int buttonID;
    bool buttonState;
}ButtonMessage;

//--------------------------------------------------
// Setup - punkt wejścia programu
//--------------------------------------------------

void setup()
{
  // Ustalenie prędkości transmisji szeregowej
  Serial.begin(115200);

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

  // Zadanie 1 - Transmisja szeregowa
  if(CREATE_TASK_SERIAL)
  {
    xTaskCreatePinnedToCore(
      TaskSerial,     // Funkcja zadania
      "Task Serial",  // Nazwa zadania
      1024,           // Rozmiar stosu
      NULL,           // Parametry funkcji
      PRIORITY_TASK_SERIAL,              // Priorytet zadania
      NULL,           // Uchwyt do zadania
      app_cpu);       // Rdzeń
  }

  // Zadanie 2 - Odczyt wejść cyfrowych
  if(CREATE_TASK_READ_DIGITAL_INPUT)
  {
    xTaskCreatePinnedToCore(
      TaskReadDigitalInput,
      "Task Read Digital Input",
      1024,
      NULL,
      PRIORITY_TASK_READ_DIGITAL_INPUT,
      NULL,
      app_cpu);
  }

  // Zadanie 3 - Odczyt wejść analogowych
  if(CREATE_TASK_READ_ANALOG_INPUT)
  {
    xTaskCreatePinnedToCore(
      TaskReadAnalogInput,
      "Task Read Analog Input",
      1024,
      NULL,
      PRIORITY_TASK_READ_ANALOG_INPUT,
      NULL,
      app_cpu);
  }
}

void loop() {}