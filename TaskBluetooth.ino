void TaskBluetooth(void *)
{
  int AnalogValues[NO_ANALOGS] = {0}; 

  ButtonMessage buttonMessage;
  AnalogMessage analogMessage;

  while(true)
  {

    if( xQueueDigital != NULL )
    {
      while( xQueueReceive( xQueueDigital, (void *)&buttonMessage, 0 ) == pdPASS )
      {
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
        }
      }
    }

    if( xQueueAnalog != NULL )
    {
      while( xQueueReceive( xQueueAnalog, (void *)&analogMessage, 0 ) == pdPASS )
      {
        if(analogMessage.analogID >= 0 && analogMessage.analogID < NO_ANALOGS)
        {
          AnalogValues[analogMessage.analogID] = analogMessage.analogVal;
        }
      }
    }
  
    // Wyślij stan gamepada

    bleGamepad.setAxes(AnalogValues[0], AnalogValues[1], AnalogValues[2], AnalogValues[3], AnalogValues[4], AnalogValues[5]);
    bleGamepad.sendReport();

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}