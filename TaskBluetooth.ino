void TaskBluetooth(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  int analogVal[NO_ANALOGS] = {0};

  ButtonMessage buttonMessage;
  AnalogMessage analogMessage;

  while(true)
  {
    if (bleGamepad.isConnected())
    {
         bleGamepad.press(BUTTON_5);
         bleGamepad.sendReport();
         vTaskDelay(500 / portTICK_PERIOD_MS);

         bleGamepad.release(BUTTON_5);
         bleGamepad.sendReport();
         vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
    vTaskDelay(500 / portTICK_PERIOD_MS);

  }
}