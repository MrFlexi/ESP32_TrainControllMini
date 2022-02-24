#include "globals.h"
#include "IRRemote.h"

IRrecv irrecv(IR_SENSOR_PIN, kCaptureBufferSize, kTimeout, true);
decode_results results; // Somewhere to store the results

void setup_irremote()
{
  irrecv.setTolerance(kTolerancePercentage); // Override the default tolerance.
  irrecv.enableIRIn();
}

void handel_irremote()
{
  if (irrecv.decode(&results))
  {
    // Display a crude timestamp.
    uint32_t now = millis();
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    yield(); // Feed the WDT as the text output can take a while to print.

    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println(); // Blank line between entries
    yield();          // Feed the WDT (again)

    Serial.println(typeToString(results.decode_type, results.repeat));
    Serial.println(results.address);
    Serial.println(results.command);

    //---------------------------------------------------------------------------------
    // React on commands
    //---------------------------------------------------------------------------------
    switch (results.command)
    {
    case 2:
      LED_sunset();
      ESP32_sleep();
#if (USE_MQTT)
      if (WiFi.status() == WL_CONNECTED)
      {
        mqtt_send();
        mqtt_send_lok(1, 22, 1);
      }
#endif
      break; // Samsung Power Button
    case 11:
      LED_sunset();
      ESP32_sleep();
      break; // Samsung Power Button
    case 4:  // Taste 1
      LED_showNumber(1);
      break; // and exits the switch
    case 5:  // Taste 2
      LED_showNumber(2);
      break; // and exits the switch
    }
  }
}