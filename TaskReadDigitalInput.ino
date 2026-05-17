void TaskReadDigitalInput(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  bool buttonPrevState[NO_BUTTONS] = {false};

  bool dpadState[NO_DPAD] = {false};
  bool dpadPrevState[NO_DPAD] = {false};

  static ButtonMessage buttonMessage;
  static DPADMessage dpadMessage;

  while(true)
  {
    for(int i = 0; i < NO_BUTTONS; i++)
    {
      // Odczyt stanu przycisku i
      buttonState[i] = !digitalRead(BUTTONS[i].pin);

      // Wykrywanie zmiany stanu przycisku i
      if(buttonState[i] != buttonPrevState[i])
      {
        // Przygotuj komunikat do kolejki o zmianie stanu przycisku
        buttonMessage.buttonID = BUTTONS[i].id; // Numer przycisku jest liczony od 1, tabela jest liczona od 0
        buttonMessage.buttonState = buttonState[i];

        if(xQueueSend( xQueueDigital, ( void * ) &buttonMessage, 10 ) != pdTRUE)
        {
          Serial.println("xQueueDigital is full.");
        }

        if(TEST_DIGITAL_INPUT)
        {
          Serial.print(BUTTONS[i].name);
          Serial.print("\tButton:\t");
          Serial.print(BUTTONS[i].id);
          Serial.print("\tPin:\t");
          Serial.print(BUTTONS[i].pin);
          Serial.print("\tState:\t");
          Serial.print(buttonState[i]);
          Serial.println();
        }
      }

      buttonPrevState[i] = buttonState[i];
    }

    for(int i = 0; i < NO_DPAD; i++)
    {
      // Odczyt stanu przycisku i
      dpadState[i] = !digitalRead(DPAD[i].pin);

      // Wykrywanie zmiany stanu przycisku i
      if(dpadState[i] != dpadPrevState[i])
      {
        // Przygotuj komunikat do kolejki o zmianie stanu przycisku
        dpadMessage.dpadID = DPAD[i].id; // Numer przycisku jest liczony od 1, tabela jest liczona od 0
        dpadMessage.dpadState = dpadState[i];

        if(xQueueSend( xQueueDPAD, ( void * ) &dpadMessage, 10 ) != pdTRUE)
        {
          Serial.println("xQueueDPAD is full.");
        }

        if(TEST_DIGITAL_INPUT)
        {
          Serial.print(DPAD[i].name);
          Serial.print("\tButton:\t");
          Serial.print(DPAD[i].id);
          Serial.print("\tPin:\t");
          Serial.print(DPAD[i].pin);
          Serial.print("\tState:\t");
          Serial.print(dpadState[i]);
          Serial.println();
        }
      }

      dpadPrevState[i] = dpadState[i];
    }
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}