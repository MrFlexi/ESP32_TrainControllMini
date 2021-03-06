#include "globals.h"
#include "mqtt.h"

#if (USE_MQTT)
PubSubClient MqttClient(wifiClient);

//const char *mqtt_server = "192.168.1.100"; // Raspberry
const char *mqtt_server = "85.209.49.65"; // Netcup
const char *mqtt_topic = "TrainControllHub/device/";
const char *mqtt_topic_mosi = "/mosi";
const char *mqtt_topic_miso = "/miso";
const char *mqtt_topic_irq = "/miso/irq";

long lastMsgAlive = 0;
long lastMsgDist = 0;

void doConcat(const char *a, const char *b, const char *c, char *out)
{
  strcpy(out, a);
  strcat(out, b);
  strcat(out, c);
}

void mqtt_loop()
{

  // MQTT Connection
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!MqttClient.connected())
    {
      ESP_LOGE(TAG, "MQTT Client not connected ");
      reconnect();
    }
    MqttClient.loop();
  }
  else
  {
    //ESP_LOGE(TAG, "Wifi not connected ");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{

  // char json[] = "{\"command\": {\"action\":\"sleep_time\", \"value\":\"10\"}}";
  //    {"comand": {
  //                   "action":"sleep_time",
  //                   "value":"10"
  //                  }
  //      }

  String message = "";

  ESP_LOGI(TAG, "MQTT message topic %s", topic);

  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  // Convert Payload to a JSON object
  StaticJsonDocument<500> doc;
  deserializeJson(doc, message);
  serializeJsonPretty(doc, Serial);

  // Check if there is a incomming command

  const char *action_in = doc["command"]["action"];
  const char *value = doc["command"]["value"];

  String action = String(action_in);
  ESP_LOGI(TAG, " action: %s  value: %s", action.c_str(), value);

#if (USE_FASTLED)
  if (action == "LED_HeatColor")
  {
    ESP_LOGI(TAG, "MQTT: LED Heat Color");
    LED_HeatColor(atoi(value));
  }

  if (action == "LED_on")
  {
    ESP_LOGI(TAG, "MQTT: LED on");
    LED_on(atoi(value));
  }

  if (action == "LED_off")
  {
    ESP_LOGI(TAG, "MQTT: LED off");
    LED_off();
  }

#endif

  if (action == "reset_gauge")
  {
    ESP_LOGI(TAG, "MQTT: Reset Coulomb Counter");
#if (HAS_PMU)
    pmu.ClearCoulombcounter();
#endif
  }

  if (action == "sleep_time")
  {
    //dataBuffer.settings.sleep_time = atoi(value);
    //ESP_LOGI(TAG, "MQTT: sleep time %2d", dataBuffer.settings.sleep_time);
    //saveConfiguration();
  }
  
}

void reconnect()
{

  char topic_in[40];
  // build MQTT topic e.g.  mrflexi/device/TBEAM-01/data
  doConcat(mqtt_topic, DEVICE_NAME, mqtt_topic_mosi, topic_in);

  int i = 0;

  if (WiFi.status() == WL_CONNECTED)
  {
    // Loop until we're reconnected
    while (!MqttClient.connected())
    {
      ESP_LOGI(TAG, "Attempting MQTT connection...");
      // Attempt to connect
      if (MqttClient.connect(DEVICE_NAME))
      {
        ESP_LOGI(TAG, "connected");
        MqttClient.publish(mqtt_topic, "connected");
        MqttClient.subscribe(topic_in);
      }
      else
      {
        ESP_LOGE(TAG, "failed, rc=");
        Serial.print(MqttClient.state());
        ESP_LOGE(TAG, " try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(500);
      }
      i++;
    }
  }
  else
    ESP_LOGE(TAG, "No Wifi connection");
}

void setup_mqtt()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    MqttClient.setServer(mqtt_server, 1883);
    MqttClient.setCallback(callback);
    MqttClient.setBufferSize(500);
    MqttClient.setSocketTimeout(120);

    if (!MqttClient.connected())
    {
      reconnect();
    }
    ESP_LOGI(TAG, "MQTT connected");    
    MqttClient.publish(mqtt_topic, "ESP32 is alive...");
  }
}


void mqtt_send()
{
  char topic_out[40];

  // build MQTT topic e.g.  mrflexi/device/TBEAM-01/data
  doConcat(mqtt_topic, DEVICE_NAME, mqtt_topic_miso, topic_out);

  if (MqttClient.connected())
  {
    //MqttClient.publish(topic_out, dataBuffer.to_json().c_str());
    ESP_LOGI(TAG, "MQTT send:  %s", topic_out);
    //ESP_LOGI(TAG, "Payload: %s", dataBuffer.to_json().c_str());
    
  }
  else
  {
    ESP_LOGE(TAG, "Mqtt not connected");
  }
}

void mqtt_send_lok(int id, uint16_t speed, int dir)
{
  char topic_out[40];
  const int capacity = JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> doc;
  String JsonStr;

  // build MQTT topic e.g.  
  doConcat(mqtt_topic, "Lok", "", topic_out);
  doc.clear();
  doc["command"] = "Lok";
  doc["id"] = id;
  doc["speed"] = speed;
  doc["dir"] = dir;
  serializeJson(doc, JsonStr);

  if (MqttClient.connected())
  {
    MqttClient.publish(topic_out, JsonStr.c_str());
    ESP_LOGI(TAG, "MQTT send:  %s", topic_out);
    ESP_LOGI(TAG, "Payload: %s", JsonStr);  
  }
  else
  {
    ESP_LOGE(TAG, "Mqtt not connected");
  }
}

void mqtt_send_irq()
{
  
}
#endif