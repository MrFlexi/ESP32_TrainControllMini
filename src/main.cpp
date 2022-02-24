
#include "globals.h"
static const char TAG[] = __FILE__;

// -------------   IR Remote Controll---------------------
// DeepSleep on Samsung IR Power Button
// WakeUp on any key

// ------------- Global DataBuffer ---------------------
deviceSettings_t gsDeviceSettings;

//--------------------------------------------------------------------------
// GPIO
//--------------------------------------------------------------------------
// GPIO 32  -->  Infra Red Sensor   --> WakeUp
// GPIO 25  -->  Fast LED Ring

//--------------------------------------------------------------------------
// Wifi Settings
//--------------------------------------------------------------------------

#if (USE_WEBSERVER || USE_CAYENNE || USE_MQTT || USE_WIFI)
WiFiClient wifiClient;
#endif


#if (USE_WEBSERVER || USE_CAYENNE || USE_MQTT || USE_WIFI )
void setup_wifi()
{
  IPAddress ip;
  // WIFI Setup
  WiFi.begin(ssid, wifiPassword);

  ESP_LOGI(TAG, "Connecting to WiFi..");
  int i = 0;  
  while ((WiFi.status() != WL_CONNECTED) && (i < 50))
  {
    delay(200);
    i++;
    Serial.print('.');
  }

  if (WiFi.status() == WL_CONNECTED)
  {    
    ip = WiFi.localIP();
    Serial.println(ip);    
  }
  else
  {
    //Turn off WiFi if no connection was made
    
    WiFi.mode(WIFI_OFF);
  }
}
#endif


//-------------------------------------------------------------------------
// SETUP
//-------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("WelcomeIR Remote Receiver"); // Blank line between entries
  print_wakeup_reason();
  gsDeviceSettings.sleep_time = 6;

#if (USE_MQTT)
  setup_wifi();
#endif

#if (USE_MQTT)
  setup_mqtt();
#endif

#if (USE_FASTLED)
  setup_FastLed();
  delay(5);
  LED_sunrise();
#endif

// IR Remote
#if (USE_IR_REMOTE)
  setup_irremote();
#endif
}

//-------------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------------
void loop()
{
  handel_irremote();
}