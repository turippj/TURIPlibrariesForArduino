#include "TURIPserverI2C.h"
#include "TURIPserver.h"
#include <Wire.h>

cl_TURIPserverI2C TURIPserverI2C;
uint8_t TURIPserverI2C_slectedPort;

cl_TURIPserverI2C::TURIPserverI2C(){
  TURIPserverI2C_slectedPort = 0;
}

void cl_TURIPserverI2C::begin(int address){
  Wire.begin(address);
  Wire.onReceive(TURIPserverI2C_onReceiveHandler);
  Wire.onRequest(TURIPserverI2C_onRequesthandler);
}

void TURIPserverI2C_onReceiveHandler(int bytes){
  TURIPserverI2C_slectedPort = Wire.read();
  if(bytes == 1 || TURIPserverI2C_slectedPort & 0x80) return;
  TURIPport* port = TURIPserver.getPort(TURIPserverI2C_slectedPort & 0x7f);
  uint8_t buffer[8] = {0};
  for (size_t i = 0; i < bytes - 1; i++) {
    buffer[i] = Wire.read();
    if (i==7) break;
  }
  if(port == NULL) return;
  port->receive(&buffer[0]);
}

void TURIPserverI2C_onRequesthandler(){
  TURIPport* port = TURIPserver.getPort(TURIPserverI2C_slectedPort & 0x7f);
  if(port == NULL) return;
  uint8_t buffer[9] = {0};
  uint8_t datatype = (uint8_t)port->type;
  uint8_t datasize = (uint8_t)port->cacheSize;
  buffer[0] = 0x00 | datatype;
  port->transmit(&buffer[1], 8);
  Wire.write(buffer, datasize + 1);
}
