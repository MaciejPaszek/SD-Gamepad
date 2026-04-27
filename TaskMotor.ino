void TaskMotor(void *)
{
  bool motorState = false;

  while(true)
  {
    motorState = !motorState;

    digitalWrite(MOT, motorState);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}