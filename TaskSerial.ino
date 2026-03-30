void TaskSerial(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  
  ButtonMessage buttonMessage;

  while(true)
  {
    if( xQueue != NULL )
    {
        if( xQueueReceive( xQueue, (void *)&buttonMessage, 0 ) == pdPASS )
        {
          // Zmień stan przycisku
          buttonState[buttonMessage.buttonID] = buttonMessage.buttonState;

          // Lista wszystkich wejść cyfrowych
          for(int i = 0; i < NO_BUTTONS; i++)
          {
            Serial.print(buttonState[i]);
          }

          // Nowa linia
          Serial.println("");
        }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}