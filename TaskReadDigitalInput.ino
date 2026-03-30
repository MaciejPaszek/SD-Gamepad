void TaskReadDigitalInput(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  bool buttonPrevState[NO_BUTTONS] = {false};

  static ButtonMessage buttonMessage;

  while(true)
  {
    for(int i = 0; i < NO_BUTTONS; i++)
    {
      // Odczyt stanu przycisku i
      buttonState[i] = !digitalRead(BUTTONS[i]);

      // Wykrywanie zmiany stanu przycisku i
      if(buttonState[i] != buttonPrevState[i])
      {
        // Przygotuj komunikat do kolejki o zmianie stanu przycisku
        buttonMessage.buttonID = i;
        buttonMessage.buttonState = buttonState[i];

        if(xQueueSend( xQueue, ( void * ) &buttonMessage, 10 ) != pdTRUE)
        {
          Serial.println("Queue is full.");
        }
      }

      buttonPrevState[i] = buttonState[i];
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}