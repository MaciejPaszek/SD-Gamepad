void TaskSerial(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  int analogVal[NO_ANALOGS] = {0};

  ButtonMessage buttonMessage;
  AnalogMessage analogMessage;

  while(true)
  {
    bool received = false;

    if( xQueue != NULL )
    {
        if( xQueueReceive( xQueue, (void *)&buttonMessage, 0 ) == pdPASS )
        {
          // Zmień stan przycisku
          buttonState[buttonMessage.buttonID] = buttonMessage.buttonState;

          received = true;
        }
    }

    if( xQueueAnalog != NULL )
    {
        if( xQueueReceive( xQueueAnalog, (void *)&analogMessage, 0 ) == pdPASS )
        {
          // Zmień stan przycisku
          analogVal[analogMessage.analogID] = analogMessage.analogVal;

          received = true;
        }
    }

    if(received == true)
    {
      printGamepadState(buttonState, analogVal);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void printGamepadState(bool * buttonState, int * analogVal)
{
  // Lista wszystkich wejść cyfrowych
  for(int i = 0; i < NO_BUTTONS; i++)
  {
    Serial.print(buttonState[i]);
  }

  // Lista wszystkich wejść analogowych
  for(int i = 0; i < NO_ANALOGS; i++)
  {
    Serial.print(" ");
    Serial.print(analogVal[i]);
  }

  // Nowa linia
  Serial.println("");
}