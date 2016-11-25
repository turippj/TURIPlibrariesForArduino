#ifndef TURIP_ON_UART_SERVER_H
#define TURIP_ON_UART_SERVER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdlib.h>
#include "TURIPserver.h"

StaticJsonBuffer<400> jsonBuffer;

class TURIPonUARTserver{
public:
  void begin();
  void put();
  void get();

  uint8_t  tstatus = 500;
  char* CLIENT_JSON;
  char* endptr;
}
