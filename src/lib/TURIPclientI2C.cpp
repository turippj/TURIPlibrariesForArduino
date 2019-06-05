#include "TURIPclientI2C.h"
#include <Wire.h>

cl_TURIPclientI2C TURIPclientI2C;

cl_TURIPclientI2C::cl_TURIPclientI2C(){
  ;
}

void cl_TURIPclientI2C::begin(){
  Wire.begin();
  TURIPclient.addTuripClientInterface(this);
}

void cl_TURIPclientI2C::scan(){
  byte error, address;
  for(address = 1; address < 127; address++ ){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      uint32_t model, serial;
      readByLocalAddress(address, TURIP_PORT_MODEL, TURIP_TYPE_INT32, &model);
      readByLocalAddress(address, TURIP_PORT_SERIAL, TURIP_TYPE_INT32, &serial);
      uint64_t id = (uint64_t)model << 32 | (uint64_t)serial;
      TURIPclient.devices[TURIPclient.numofDevices].id = id;
      TURIPclient.devices[TURIPclient.numofDevices].interface = this;
      TURIPclient.devices[TURIPclient.numofDevices].localAddr = address;
      TURIPclient.numofDevices++;
    }
  }
}

int cl_TURIPclientI2C::readByLocalAddress(uint8_t localAddress, uint8_t port, TURIPdataType type, void* data){
  int datasize;
  switch (type) {
    case TURIP_TYPE_BOOL:
    case TURIP_TYPE_INT8:
    case TURIP_TYPE_UINT8:
      datasize = 1;
      break;
    case TURIP_TYPE_INT16:
    case TURIP_TYPE_UINT16:
      datasize = 2;
      break;
    case TURIP_TYPE_INT32:
    case TURIP_TYPE_UINT32:
    case TURIP_TYPE_FLOAT:
      datasize = 4;
      break;
    case TURIP_TYPE_INT64:
    case TURIP_TYPE_UINT64:
    case TURIP_TYPE_DOUBLE:
      datasize = 8;
      break;
    default:
      datasize = 0;
  }
  Wire.beginTransmission(localAddress);
  Wire.write(port);
  Wire.endTransmission();
  size_t received_nbytes = Wire.requestFrom(localAddress, datasize + 1);
  uint8_t status = Wire.read();
  if((status & 0xf0) != 0x00) {
    return TURIP_ERR;
  }
  for (size_t i = 0; i < datasize; i++) {
    *((char*)data + i) = Wire.read();
  }
  return TURIP_OK;
}

int cl_TURIPclientI2C::writeByLocalAddress(uint8_t localAddress, uint8_t port, TURIPdataType type, void* data){
  int datasize;
  switch (type) {
    case TURIP_TYPE_BOOL:
    case TURIP_TYPE_INT8:
    case TURIP_TYPE_UINT8:
      datasize = 1;
      break;
    case TURIP_TYPE_INT16:
    case TURIP_TYPE_UINT16:
      datasize = 2;
      break;
    case TURIP_TYPE_INT32:
    case TURIP_TYPE_UINT32:
    case TURIP_TYPE_FLOAT:
      datasize = 4;
      break;
    case TURIP_TYPE_INT64:
    case TURIP_TYPE_UINT64:
    case TURIP_TYPE_DOUBLE:
      datasize = 8;
      break;
    default:
      datasize = 0;
  }

  Wire.beginTransmission(localAddress);
  Wire.write(port);
  for (size_t i = 0; i < datasize; i++) {
    Wire.write(*((char*)data + i));
  }
  Wire.endTransmission();
  return TURIP_OK;
}

int cl_TURIPclientI2C::read(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  int localAddress = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      localAddress = TURIPclient.devices[i].localAddr;
    }
  }
  if(localAddress == -1){
    return TURIP_ERR;
  }
  return readByLocalAddress(localAddress, port, type, data);
}

int cl_TURIPclientI2C::write(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  int localAddress = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      localAddress = TURIPclient.devices[i].localAddr;
    }
  }
  if(localAddress == -1){
    return TURIP_ERR;
  }
  return writeByLocalAddress(localAddress, port, type, data);
}

TURIPdataType cl_TURIPclientI2C::getType(uint64_t id, uint8_t port){
  int localAddress = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      localAddress = TURIPclient.devices[i].localAddr;
    }
  }
  if(localAddress == -1){
    return TURIP_ERR;
  }
  Wire.beginTransmission(localAddress);
  Wire.write(port);
  Wire.endTransmission();
  size_t received_nbytes = Wire.requestFrom(localAddress, 1);
  uint8_t status = Wire.read();
  if((status & 0xf0) != 0x00) {
    return TURIP_TYPE_UNKNOWN;
  }
  return (TURIPdataType)(status & 0x0f);
}
