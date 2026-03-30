void TaskBluetooth(void *)
{
  ButtonMessage buttonMessage;
  AnalogMessage analogMessage;

  while(true)
  {

    if( xQueue != NULL )
    {
      if( xQueueReceive( xQueue, (void *)&buttonMessage, 0 ) == pdPASS )
      {
        Serial.println(buttonMessage.buttonID);

        if (bleGamepad.isConnected())
        {
          if(buttonMessage.buttonState == true)
          {
            bleGamepad.press(buttonMessage.buttonID);
          }
          else
          {
            bleGamepad.release(buttonMessage.buttonID);
          }

          bleGamepad.sendReport();
        }
      }
    }

    if( xQueueAnalog != NULL )
    {
      if( xQueueReceive( xQueueAnalog, (void *)&analogMessage, 0 ) == pdPASS )
      {
        // Zmień stan przycisku
        Serial.println(analogMessage.analogVal);
      }
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}