#ifndef TURIP_CLIENT_I2C_H
#define TURIP_CLIENT_I2C_H

#include <Arduino.h>
#include "TURIPclient.h"

class cl_TURIPclientI2C : public turipClientInterface{
public:
  cl_TURIPclientI2C();
  void begin();
  virtual void scan();
  virtual int write(uint64_t id, uint8_t port, TURIPdataType type, void* data);
  virtual int read(uint64_t id, uint8_t port, TURIPdataType type, void* data);
  virtual TURIPdataType getType(uint64_t id, uint8_t port);
  int readByLocalAddress(uint8_t localAddress, uint8_t port, TURIPdataType type, void* data);
  int writeByLocalAddress(uint8_t localAddress, uint8_t port, TURIPdataType type, void* data);
private:

};

extern cl_TURIPclientI2C TURIPclientI2C;

#endif
