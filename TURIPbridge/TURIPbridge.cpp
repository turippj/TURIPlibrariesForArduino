#include "TURIPbridge.h"

cl_TURIPbridge TURIPbridge;

void cl_TURIPbridge::begin(){
  portOfMumofDev = new TURIPport(72, INT8);
  portOfSelectedDeviceModel = new TURIPport(73, INT32);
  portOfSelectedDeviceSerial = new TURIPport(74, INT32);
  portOfPortType = new TURIPport(75, INT8);
  portOfDeviceSelector = new TURIPport(76, INT8);
  portOfPortSelector = new TURIPport(77, INT8);
  portOfData = new TURIPport(78, INT8);

  portOfMumofDev->setReadProcess(TURIPbridgeFuncs::scanDevices);
  portOfDeviceSelector->setWriteProcess(TURIPbridgeFuncs::setDevice);
  portOfPortSelector->setWriteProcess(TURIPbridgeFuncs::setPort);
  portOfData->setReadProcess(TURIPbridgeFuncs::getDataOfBridge);
  portOfData->setWriteProcess(TURIPbridgeFuncs::setDataOfBridge);
}

int TURIPbridgeFuncs::scanDevices(){
  return 0;
}

int TURIPbridgeFuncs::setDevice(){
  return 0;
}

int TURIPbridgeFuncs::setPort(){
  return 0;
}

int TURIPbridgeFuncs::getDataOfBridge(){
  return 0;
}

int TURIPbridgeFuncs::setDataOfBridge(){
  return 0;
}
