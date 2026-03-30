void TaskReadAnalogInput(void *)
{
  int analogVal[NO_ANALOGS] = {0};

  while(true)
  {
    // Odczyt stanów przycisków
    for(int i = 0; i < NO_ANALOGS; i++)
    {
      // Odczyt wartośći wejścia i
      analogVal[i] = analogRead(ANALOGS[i]);

      Serial.print("Analog ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(analogVal[i]);
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}