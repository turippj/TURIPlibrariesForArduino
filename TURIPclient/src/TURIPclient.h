#ifndef TURIP_CLIENT_H
#define TURIP_CLIENT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <TURIPcommon.h>

class TURIPclientPeripheral{
public:
  TURIPclientPeripheral();
  virtual int scan(uint64_t* idList[]);
  virtual int write(uint64_t id, int port, TURIPdataType type, void* data);
  virtual int read(uint64_t id, int port, TURIPdataType type, void* data);
  virtual TURIPdataType getType(uint64_t id, int port);
  virtual int isExist(uint64_t id);
protected:
  uint64_t idList[16];
  int numofDevices;
};

class TURIP{
public:
  TURIP();
  int attach(uint64_t id);
  int read(int port, TURIPdataType type, void* data);
  int write(int port, TURIPdataType type, void* data);
  TURIPdataType getType(int port);
private:
  uint64_t id;
  TURIPclientPeripheral* peripheral;
};

class cl_TURIPclient{
public:
  cl_TURIPclient();
  int addPeripheral(TURIPclientPeripheral* peripheral);
  TURIPclientPeripheral* getPeripheral(uint64_t id);
  int scan();
  int scan(uint64_t* idList[]);
  int isExist(uint64_t id);
  uint64_t idList[16];
  int numofDevices;
private:
  TURIPclientPeripheral* peripheralList[4];
  size_t numofPeripherals;
};
extern cl_TURIPclient TURIPclient;

#endif
