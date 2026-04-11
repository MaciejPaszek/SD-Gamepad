void TaskReadGyro(void *)
{
  //sensors_event_t a, g, temp;

  Serial.println("Start TaskReadGyro()");
  while(true)
  {
    Serial.println("TaskReadGyro()");

    //Mpu.getEvent(&a, &g, &temp);

    //Serial.print(a.acceleration.x);
    //Serial.print(" ");
    //Serial.print(a.acceleration.y);
    //Serial.print(" ");
    //Serial.print(a.acceleration.z);
    //Serial.println();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}