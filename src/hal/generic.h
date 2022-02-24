#pragma once

#include <stdint.h>

// Hardware related definitions for generic ESP32 boards
// generic.h is kitchensink with all available options

#define HAS_SPI 1  // comment out if device shall not send data via SPI
// pin definitions for SPI slave interface
#define SPI_MOSI GPIO_NUM_23
#define SPI_MISO GPIO_NUM_19
#define SPI_SCLK GPIO_NUM_18
#define SPI_CS   GPIO_NUM_5

#define FASTLED_DATA_PIN GPIO_NUM_25

#define HAS_REMOTE_IR_SENSOR 1
#define IR_SENSOR_PIN GPIO_NUM_32

// BME280 sensor on I2C bus
//#define HAS_BME 1 // Enable BME sensors in general
//#define HAS_BME280 GPIO_NUM_21, GPIO_NUM_22 // SDA, SCL
//#define BME280_ADDR 0x76 // change to 0x77 depending on your wiring

// user defined sensors
//#define HAS_SENSORS 1 // comment out if device has user defined sensors

// #define BOARD_HAS_PSRAM // use if board has external PSRAM
#define DISABLE_BROWNOUT 1 // comment out if you want to keep brownout feature

#define HAS_DISPLAY U8X8_SSD1306_128X64_NONAME_HW_I2C
//#define DISPLAY_FLIP  1 // use if display is rotated

#define BAT_VOLTAGE_DIVIDER 2 // voltage divider 100k/100k on board

#define HAS_LED (21) // on board  LED
#define BUTTON_PIN (39) // on board button

// GPS settings
#define HAS_GPS 0 // use on board GPS
#define GPS_SERIAL 9600, SERIAL_8N1, GPIO_NUM_12, GPIO_NUM_15 // UBlox NEO 6M RX, TX
#define GPS_INT GPIO_NUM_13 // 30ns accurary timepulse, to be external wired on pcb: NEO 6M Pin#3 -> GPIO13

// Pins for I2C interface of OLED Display
#define MY_OLED_SDA (4)
#define MY_OLED_SCL (15)
#define MY_OLED_RST (16)

// Settings for on board DS3231 RTC chip
#define HAS_RTC MY_OLED_SDA, MY_OLED_SCL // SDA, SCL
#define RTC_INT GPIO_NUM_34 // timepulse with accuracy +/- 2*e-6 [microseconds] = 0,1728sec / day


// I2C config for Microchip 24AA02E64 DEVEUI unique address
#define MCP_24AA02E64_I2C_ADDRESS 0x50 // I2C address for the 24AA02E64 
#define MCP_24AA02E64_MAC_ADDRESS 0xF8 // Memory adress of unique deveui 64 bits
