void TaskBluetooth(void *)
{
  int AnalogValues[NO_ANALOGS] = {0}; 

  ButtonMessage buttonMessage;
  DPADMessage dpadMessage;
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

    if( xQueueDPAD != NULL )
    {
      while( xQueueReceive( xQueueDPAD, (void *)&dpadMessage, 0 ) == pdPASS )
      {
        if (bleGamepad.isConnected())
        {
          if(dpadMessage.dpadState == true)
          {
            bleGamepad.setHat1(DPAD_DOWN_RIGHT);
          }
          else
          {
            bleGamepad.setHat1(DPAD_UP_LEFT);
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

    // Lewa gałka analogowa
    bleGamepad.setX(AnalogValues[0]);
    bleGamepad.setY(AnalogValues[1]);

    // Prawa gałka analogowa
    bleGamepad.setZ(AnalogValues[2]);
    bleGamepad.setRZ(AnalogValues[3]);

    // Lewy trigger
    bleGamepad.setRX(AnalogValues[4]);

    // Prawy trigger
    bleGamepad.setRY(AnalogValues[5]);

    // Wysyłanie
    bleGamepad.sendReport();

    // Opóźnienie
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}