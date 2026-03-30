void TaskBluetooth(void *)
{
  bool buttonState[NO_BUTTONS] = {false};
  int analogVal[NO_ANALOGS] = {0};

  ButtonMessage buttonMessage;
  AnalogMessage analogMessage;

  while(true)
  {
    // if (bleGamepad.isConnected())
    // {
    //     Serial.println("Press buttons 1, 32, 64 and 128. Set hat 1 to down right and hat 2 to up left");

    //     // Press buttons 5, 32, 64 and 128
    //     bleGamepad.press(BUTTON_5);
    //     bleGamepad.press(BUTTON_32);
    //     bleGamepad.press(BUTTON_64);
    //     bleGamepad.press(BUTTON_128);

    //     // Move all axes to max.
    //     bleGamepad.setLeftThumb(32767, 32767);  // or bleGamepad.setX(32767); and bleGamepad.setY(32767);
    //     bleGamepad.setRightThumb(32767, 32767); // or bleGamepad.setZ(32767); and bleGamepad.setRZ(32767);
    //     bleGamepad.setLeftTrigger(32767);       // or bleGamepad.setRX(32767);
    //     bleGamepad.setRightTrigger(32767);      // or bleGamepad.setRY(32767);
    //     bleGamepad.setSlider1(32767);
    //     bleGamepad.setSlider2(32767);

    //     // Set hat 1 to down right and hat 2 to up left (hats are otherwise centered by default)
    //     bleGamepad.setHat1(DPAD_DOWN_RIGHT); // or bleGamepad.setHat1(HAT_DOWN_RIGHT);
    //     bleGamepad.setHat2(DPAD_UP_LEFT);    // or bleGamepad.setHat2(HAT_UP_LEFT);
    //     // Or bleGamepad.setHats(DPAD_DOWN_RIGHT, DPAD_UP_LEFT);

    //     // Send the gamepad report
    //     bleGamepad.sendReport();
    //     vTaskDelay(500 / portTICK_PERIOD_MS);

    //     Serial.println("Release button 5 and 64. Move all axes to min. Set hat 1 and 2 to centred.");
    //     bleGamepad.release(BUTTON_5);
    //     bleGamepad.release(BUTTON_64);
    //     bleGamepad.setAxes(0, 0, 0, 0, 0, 0, 0, 0);
    //     bleGamepad.setHats(DPAD_CENTERED, HAT_CENTERED);
    //     bleGamepad.sendReport();
    //     vTaskDelay(500 / portTICK_PERIOD_MS);
    // }


    // bool received = false;

    // if( xQueue != NULL )
    // {
    //     if( xQueueReceive( xQueue, (void *)&buttonMessage, 0 ) == pdPASS )
    //     {
    //       if (bleGamepad.isConnected())
    //       {
    //         if(buttonMessage.buttonState == true)
    //         {
    //           bleGamepad.press(buttonMessage.buttonID);
    //         }
    //         else
    //         {
    //           bleGamepad.release(buttonMessage.buttonID);
    //         }
    //       }

    //       // Zmień stan przycisku
    //       buttonState[buttonMessage.buttonID] = buttonMessage.buttonState;

    //       received = true;
    //     }
    // }

    // if( xQueueAnalog != NULL )
    // {
    //     if( xQueueReceive( xQueueAnalog, (void *)&analogMessage, 0 ) == pdPASS )
    //     {
    //       // Zmień stan przycisku
    //       analogVal[analogMessage.analogID] = analogMessage.analogVal;

    //       received = true;
    //     }
    // }

    // if(received == true)
    // {
    //   printGamepadState(buttonState, analogVal);
    // }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}