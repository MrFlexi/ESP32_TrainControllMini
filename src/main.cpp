
#include "globals.h"
static const char TAG[] = __FILE__;

// -------------   IR Remote Controll---------------------
// DeepSleep on Samsung IR Power Button
// WakeUp on any key


// -------------   IR Remote Controll---------------------
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 15;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance; // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false
IRrecv irrecv(IR_SENSOR_PIN, kCaptureBufferSize, kTimeout, true);
decode_results results; // Somewhere to store the results


// ------------- Global DataBuffer ---------------------
deviceSettings_t gsDeviceSettings;

//--------------------------------------------------------------------------
// GPIO
//--------------------------------------------------------------------------
// GPIO 32  -->  Infra Red Sensor   --> WakeUp
// GPIO 25  -->  Fast LED Ring

void setup()
{
  Serial.begin(115200);
  Serial.println("WelcomeIR Remote Receiver"); // Blank line between entries
  print_wakeup_reason();

  gsDeviceSettings.sleep_time = 6;

#if (USE_FASTLED)
  setup_FastLed();
  delay(5);
  LED_boot();
#endif

  irrecv.setTolerance(kTolerancePercentage); // Override the default tolerance.
  irrecv.enableIRIn();                       // Start the receiver
}

void loop()
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

    switch (results.command)
    {
    case 2:
      LED_sunset();
      ESP32_sleep();
      break; // Samsung Power Button
    case 11:
      LED_sunset();
      ESP32_sleep();
      break; // Samsung Power Button
    case 4:
      LED_showDegree(1);
      break; // and exits the switch
    case 5:
      LED_showDegree(200);
      break; // and exits the switch
    }
  }
}