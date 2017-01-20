#include "TURIPbridge.h"

#include <Arduino.h>

cl_TURIPbridge TURIPbridge;

void cl_TURIPbridge::begin(){
  portOfMumofDev = new TURIPport(80, INT8);
  portOfSelectedDeviceModel = new TURIPport(81, INT32);
  portOfSelectedDeviceSerial = new TURIPport(82, INT32);
  portOfPortType = new TURIPport(83, INT8);
  portOfDeviceSelector = new TURIPport(84, INT8);
  portOfPortSelector = new TURIPport(85, INT8);
  portOfData = new TURIPport(86, INT8);

  portOfMumofDev->setReadProcess(TURIPbridgeFuncs::scanDevices);
  portOfDeviceSelector->setWriteProcess(TURIPbridgeFuncs::setDevice);
  // portOfPortSelector->setWriteProcess(TURIPbridgeFuncs::setPort);
  portOfPortType->setWriteProcess(TURIPbridgeFuncs::setPort);
  portOfData->setReadProcess(TURIPbridgeFuncs::getDataOfBridge);
  portOfData->setWriteProcess(TURIPbridgeFuncs::setDataOfBridge);

  activeDevice = NULL;
  portOfMumofDev->write<uint8_t>(0);
}

int TURIPbridgeFuncs::scanDevices(){
  TURIPclient.scan();
  TURIPbridge.portOfMumofDev->write<uint8_t>(TURIPclient.numofDevices);
  return 0;
}

int TURIPbridgeFuncs::setDevice(){
  TURIPbridge.selectedDevId = TURIPclient.idList[TURIPbridge.portOfDeviceSelector->read<uint8_t>()];
  uint32_t deviceNum = (uint32_t)TURIPbridge.selectedDevId;
  uint32_t serialNum = (uint32_t)(TURIPbridge.selectedDevId >> 32);
  TURIPbridge.portOfSelectedDeviceModel->write<uint32_t>(deviceNum);
  TURIPbridge.portOfSelectedDeviceSerial->write<uint32_t>(serialNum);
  if (TURIPbridge.activeDevice!=NULL){
    delete TURIPbridge.activeDevice;
  }
  TURIPbridge.activeDevice = new TURIP();
  return TURIPbridge.activeDevice->attach(TURIPbridge.selectedDevId);
}

int TURIPbridgeFuncs::setPort(){
  if(TURIPbridge.activeDevice!=NULL){
    TURIPbridge.portOfData->setType((TURIPdataType)TURIPbridge.portOfPortType->read<uint8_t>());
    return 0;
  }
  return -1;
}

int TURIPbridgeFuncs::getDataOfBridge(){
  uint8_t databuf[8];
  TURIPdataType portType = (TURIPdataType)TURIPbridge.portOfPortType->read<uint8_t>();
  uint8_t portNum = TURIPbridge.portOfPortSelector->read<uint8_t>();
  if(!TURIPbridge.activeDevice->read(portNum, portType, databuf)){
    switch(portType){
      case INT8:
      TURIPbridge.portOfData->write<uint8_t>(*(uint8_t*)databuf);
      break;
      case INT16:
      TURIPbridge.portOfData->write<uint16_t>(*(uint16_t*)databuf);
      break;
      case INT32:
      TURIPbridge.portOfData->write<uint32_t>(*(uint32_t*)databuf);
      break;
      case INT64:
      TURIPbridge.portOfData->write<uint64_t>(*(uint64_t*)databuf);
      break;
      case FLOAT:
      TURIPbridge.portOfData->write<float>(*(float*)databuf);
      break;
      case DOUBLE:
      TURIPbridge.portOfData->write<double>(*(double*)databuf);
      break;
      case BOOL:
      TURIPbridge.portOfData->write<uint8_t>(*(uint8_t*)databuf);
      break;
    }
    return 0;
  }
  return -1;
}

int TURIPbridgeFuncs::setDataOfBridge(){
  int status = -1;
  uint8_t portNum = TURIPbridge.portOfPortSelector->read<uint8_t>();
  TURIPdataType portType = (TURIPdataType)TURIPbridge.portOfPortType->read<uint8_t>();
  uint8_t bufInt8;
  uint16_t bufInt16;
  uint32_t bufInt32;
  uint64_t bufInt64;
  float bufFloat;
  double bufDouble;
  switch(portType){
    case INT8:
    bufInt8 = TURIPbridge.portOfData->read<uint8_t>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufInt8);
    break;
    case INT16:
    bufInt16 = TURIPbridge.portOfData->read<uint16_t>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufInt16);
    break;
    case INT32:
    bufInt32 = TURIPbridge.portOfData->read<uint32_t>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufInt32);
    break;
    case INT64:
    bufInt64 = TURIPbridge.portOfData->read<uint64_t>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufInt64);
    break;
    case FLOAT:
    bufFloat = TURIPbridge.portOfData->read<float>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufFloat);
    break;
    case DOUBLE:
    bufDouble = TURIPbridge.portOfData->read<double>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufDouble);
    break;
    case BOOL:
    bufInt8 = TURIPbridge.portOfData->read<uint8_t>();
    status = TURIPbridge.activeDevice->write(portNum, portType, &bufInt8);
    break;
  }
  return status;
}
