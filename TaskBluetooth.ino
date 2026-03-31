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
      while( xQueueReceive( xQueueAnalog, (void *)&analogMessage, 0 ) == pdPASS )
      {
        // Zmień stan przycisku
        Serial.println(analogMessage.analogVal);

        if(analogMessage.analogID == 0)
        {
          if (bleGamepad.isConnected())
          {
            bleGamepad.setX(analogMessage.analogVal);
            bleGamepad.sendReport();
          }
        }

        if(analogMessage.analogID == 1)
        {
          if (bleGamepad.isConnected())
          {
            bleGamepad.setY(analogMessage.analogVal);
            bleGamepad.sendReport();
          }
        }

      }
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}