#ifndef TURIP_BRIDGE_H
#define TURIP_BRIDGE_H

#include "TURIPserver.h"
#include "TURIPclient.h"

class cl_TURIPbridge{
public:
  TURIPport* portOfMumofDev;
  TURIPport* portOfSelectedDeviceID;
  TURIPport* portOfPortType;
  TURIPport* portOfDeviceSelector;
  TURIPport* portOfPortSelector;
  TURIPport* portOfData;
  void begin();

  TURIP* activeDevice;
  uint64_t selectedDevId;
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
