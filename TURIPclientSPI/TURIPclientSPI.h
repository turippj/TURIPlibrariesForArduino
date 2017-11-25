#ifndef TURIP_ON_SPI_CLIENT_H
#define TURIP_ON_SPI_CLIENT_H

#include <Arduino.h>
#include "TURIPclient.h"

#define TURIP_ON_SPI_CLIENT_TRANS_DELAY 100
#define TURIP_ON_SPI_CLIENT_TRANS_RETRY 512

class cl_TURIPclientSPI : public TURIPclientPeripheral{
public:
  cl_TURIPclientSPI();
  void begin();
  void addPin(int ssPin);
  virtual int scan(uint64_t* idList[]);
  virtual int write(uint64_t id, int port, TURIPdataType type, void* data);
  virtual int read(uint64_t id, int port, TURIPdataType type, void* data);
  virtual TURIPdataType getType(uint64_t id, int port);
  virtual int isExist(uint64_t id);
  int readByUseSS(int ss, int port, TURIPdataType type, void* data);

private:
  int ssPinList[16];
  int numOfSsPin;
  int idToSsPinList[16];

  int getSsPin(uint64_t id);
};
extern cl_TURIPclientSPI TURIPclientSPI;

#endif
