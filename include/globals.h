
#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>
#include "SPIFFS.h"
#include <SPI.h>
#include "esp_log.h"
#include <Wire.h>
//#include <FreeRTOS.h>       // Contains Bluetooth BLE Library 
//#include "esp_system.h"
//#include "esp_spi_flash.h"

//--------------------------------------------------------------------------
// Hardware Abstraction Layer  
//--------------------------------------------------------------------------
#include "../src/hal/generic.h"


#define WDT_TIMEOUT 10          // Watchdog time out x seconds
#define uS_TO_S_FACTOR 1000000UL  //* Conversion factor for micro seconds to seconds */
#define SEALEVELPRESSURE_HPA (1013.25)

#define DEVICE_NAME "TrainHub-01"
#define USE_IR_REMOTE 1
#define USE_FASTLED 1
#define USE_MQTT 1

//--------------------------------------------------------------------------
// Wifi Settings
//--------------------------------------------------------------------------
#if (USE_WEBSERVER || USE_CAYENNE || USE_MQTT || USE_WIFI )
#include "WiFi.h"
extern WiFiClient wifiClient;
#endif

const char ssid[] = "MrFlexi";
const char wifiPassword[] = "Linde-123";

#if (USE_IR_REMOTE)
#include "IRRemote.h"
#endif

#if (USE_WEBSERVER || USE_CAYENNE || USE_MQTT || USE_WIFI )
#include "WiFi.h"
extern WiFiClient wifiClient;
#endif

#if (USE_SERVO)
#include <ESP32Servo.h>
// Published values for SG90 servos; adjust if needed
extern int minUs;
extern int maxUs;

#ifdef SERVO1_PIN
extern Servo servo1;
#endif

#ifdef SERVO2_PIN
extern Servo servo2;
#endif

#endif

#if (USE_WEBSERVER)
#include "ESPAsyncWebServer.h"
#include "webserver.h"
#include "websocket.h"
#endif


typedef struct
{
  float iaq;                // IAQ signal
  uint8_t iaq_accuracy;     // accuracy of IAQ signal
  float temperature;        // temperature signal
  float humidity;           // humidity signal
  float pressure;           // pressure signal
  float cpu_temperature;    // raw temperature signal
  float raw_temperature;    // raw temperature signal
  float raw_humidity;       // raw humidity signal
  float gas;                // raw gas sensor signal
  uint8_t aliveCounter;     // aliveCounter
  uint8_t LoraQueueCounter; // aliveCounter
  uint8_t sleepCounter;     // aliveCounter
  uint8_t MotionCounter;     // aliveCounter
  uint16_t bootCounter;
  uint8_t txCounter;        // aliveCounter
   uint8_t rxCounter;        // aliveCounter
  uint8_t runmode;          // aliveCounter
  uint32_t freeheap;        // free memory
  uint8_t tx_ack_req;       // request TTN to acknowlede a TX
  uint16_t potentiometer_a;   //
  bool potentiometer_a_changed;
  uint32_t bat_ChargeCoulomb = 0;
  uint32_t bat_DischargeCoulomb = 0;
  float    bat_DeltamAh = 0;
  uint8_t  bat_max_charge_curr = 0;
  bool  wlan;
  bool  pictureLoop = true;
  float firmware_version;
  bool pmu_data_available;
  float panel_voltage = 0;
  float panel_current = 0;
  float bus_voltage = 0;
  float bus_current = 0;
  float bat_voltage = 0;
  float bat_charge_current = 0;
  float bat_discharge_current = 0;
  double yaw = 0;
  double pitch = 0;
  double roll = 0;
  String ip_address;
  uint8_t operation_mode = 0;
  esp_sleep_wakeup_cause_t wakeup_reason;
  char gps_datetime[32];
} deviceStatus_t;


typedef struct
{
  uint8_t sleep_time;
  uint8_t bat_max_charge_current;
  
} deviceSettings_t;


extern deviceSettings_t gsDeviceSettings;

extern deviceStatus_t deviceSettings;
extern SemaphoreHandle_t I2Caccess;

extern TaskHandle_t irqHandlerTask;
extern TaskHandle_t moveDisplayHandlerTask;
extern TaskHandle_t t_cyclic_HandlerTask;

extern QueueHandle_t LoraSendQueue;


#include "power.h"

#if (HAS_INA3221 || HAS_INA219 || USE_BME280 )
#include "i2c_sensors.h"
#endif

#if (USE_MQTT)
#include "mqtt.h"
#endif

#if (USE_BUTTON)
#include "button.h"
#endif

#if (HAS_LORA)
#include "lora.h"
#endif

#if (USE_OTA)
#include "SecureOTA.h"
#endif


#if (USE_FASTLED)
#include <Led.h>
#endif

#if (USE_POTI)
#include "poti.h"
#endif

#endif