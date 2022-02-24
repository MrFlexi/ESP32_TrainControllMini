#pragma once

#include <Arduino.h>

#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

// -------------   IR Remote Controll---------------------
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 15;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance; // kTolerance is normally 25%
#define LEGACY_TIMING_INFO false



void handel_irremote();
void setup_irremote();