void TaskReadAnalogInput(void *)
{
  int analogVal[NO_ANALOGS] = {0};
  int analogPrevVal[NO_ANALOGS] = {0};

  static AnalogMessage analogMessage;

  while(true)
  {
    for(int i = 0; i < NO_ANALOGS; i++)
    {
      // Odczyt analoga i
      analogVal[i] = analogRead(ANALOGS[i]);

      // Wykrywanie zmiany odczytu analoga
      if(analogVal[i] != analogPrevVal[i])
      {
        // Przygotuj komunikat do kolejki o zmianie odczytu analoga
        analogMessage.analogID = i;
        analogMessage.analogVal = analogVal[i];

        if(xQueueSend( xQueueAnalog, ( void * ) &analogMessage, 10 ) != pdTRUE)
        {
          Serial.println("QueueAnalog is full.");
        }
      }

      analogPrevVal[i] = analogVal[i];
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}