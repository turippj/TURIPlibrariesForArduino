#ifndef TURIP_BRIDGE_H
#define TURIP_BRIDGE_H

#include "TURIPserver.h"
#include "TURIPclient.h"

class cl_TURIPbridge{
public:
  TURIPport* portOfMumofDev;
  TURIPport* portOfSelectedDeviceModel;
  TURIPport* portOfSelectedDeviceSerial;
  TURIPport* portOfPortType;
  TURIPport* portOfDeviceSelector;
  TURIPport* portOfPortSelector;
  TURIPport* portOfData;
  void begin();

private:
  uint32_t idList[2][16];
  TURIP* deviceToBridge;
};
extern  cl_TURIPbridge TURIPbridge;

namespace TURIPbridgeFuncs{
  int scanDevices();
  int setDevice();
  int setPort();
  int getDataOfBridge();
  int setDataOfBridge();
}

#endif
