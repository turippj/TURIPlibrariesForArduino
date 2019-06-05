#ifndef TURIP_SERVER_I2C_H
#define TURIP_SERVER_I2C_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/defines.h"
#include "lib/turipPort.h"

class cl_TURIPserverI2C{
public:
  TURIPserverI2C();
  void begin(int address);
};

void TURIPserverI2C_onReceiveHandler(int bytes);
void TURIPserverI2C_onRequesthandler();

extern uint8_t TURIPserverI2C_slectedPort;
extern cl_TURIPserverI2C TURIPserverI2C;

#endif
