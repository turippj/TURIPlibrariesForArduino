#include "TURIPclient.h"
#include <TURIPcommon.h>

cl_TURIPclient TURIPclient;

cl_TURIPclient::cl_TURIPclient(){
  numofInterfaces = 0;
  numofDevices = 0;
}

void cl_TURIPclient::addTuripClientInterface(turipClientInterface* interface){
  interfaces[numofInterfaces++] = interface;
}

void cl_TURIPclient::scan(){
  numofDevices = 0;
  for(int i = 0; i < numofInterfaces; i++){
    interfaces[i]->scan();
  }
}

turipDeviceAddr cl_TURIPclient::search(uint64_t id){
  for(int i = 0; i < numofDevices; i++){
    if(devices[i].id == id) return devices[i];
  }
  turipDeviceAddr noDevice;
  noDevice.id = 0;
  noDevice.interface = NULL;
  return noDevice;
}





TURIPdevice::TURIPdevice(){
  addr.id = 0;
  addr.interface = NULL;
}

int TURIPdevice::attach(uint64_t id){
  addr = TURIPclient.search(id);
  if(addr.interface == NULL){
    TURIPclient.scan();
    addr = TURIPclient.search(id);
    if(addr.interface == NULL) return TURIP_ERR;
  }
  return TURIP_OK;
}

TURIPdataType TURIPdevice::getType(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  return addr.interface->getType(addr.id, port);
}

int TURIPdevice::read(uint8_t port, TURIPdataType type, void* data){
  if(addr.interface == NULL) return TURIP_ERR;
  return addr.interface->read(addr.id, port, type, data);
}

int TURIPdevice::write(uint8_t port, TURIPdataType type, void* data){
  if(addr.interface == NULL) return TURIP_ERR;
  return addr.interface->write(addr.id, port, type, data);
}



int8_t TURIPdevice::readInt8(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  int8_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_INT8, &data);
  return data;
}

int16_t TURIPdevice::readInt16(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  uint8_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_INT16, &data);
  return data;
}

int32_t TURIPdevice::readInt32(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  int32_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_INT32, &data);
  return data;
}

int64_t TURIPdevice::readInt64(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  int64_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_INT64, &data);
  return data;
}

uint8_t TURIPdevice::readUint8(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  uint8_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_UINT8, &data);
  return data;
}

uint16_t TURIPdevice::readUint16(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  uint16_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_UINT16, &data);
  return data;
}

uint32_t TURIPdevice::readUint32(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  uint32_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_UINT32, &data);
  return data;
}

uint64_t TURIPdevice::readUint64(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  uint64_t data;
  addr.interface->read(addr.id, port, TURIP_TYPE_UINT64, &data);
  return data;
}

float TURIPdevice::readFloat(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  float data;
  addr.interface->read(addr.id, port, TURIP_TYPE_FLOAT, &data);
  return data;
}

double TURIPdevice::readDouble(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  double data;
  addr.interface->read(addr.id, port, TURIP_TYPE_DOUBLE, &data);
  return data;
}

String TURIPdevice::readString(uint8_t port){
  if(addr.interface == NULL) return TURIP_ERR;
  String data;
  addr.interface->read(addr.id, port, TURIP_TYPE_STRING, &data);
  return data;
}



void TURIPdevice::writeInt8(uint8_t port, int8_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_INT8, &data);
}

void TURIPdevice::writeInt16(uint8_t port, int16_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_INT16, &data);
}

void TURIPdevice::writeInt32(uint8_t port, int32_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_INT32, &data);
}

void TURIPdevice::writeInt64(uint8_t port, int64_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_INT64, &data);
}

void TURIPdevice::writeUint8(uint8_t port, uint8_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_UINT8, &data);
}

void TURIPdevice::writeUint16(uint8_t port, uint16_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_UINT16, &data);
}

void TURIPdevice::writeUint32(uint8_t port, uint32_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_UINT32, &data);
}

void TURIPdevice::writeUint64(uint8_t port, uint64_t data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_UINT64, &data);
}

void TURIPdevice::writeFloat(uint8_t port, float data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_FLOAT, &data);
}

void TURIPdevice::writeDouble(uint8_t port, double data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_DOUBLE, &data);
}

void TURIPdevice::writeString(uint8_t port, String data){
  if(addr.interface == NULL) return TURIP_ERR;
  addr.interface->write(addr.id, port, TURIP_TYPE_STRING, &data);
}





void turipClientInterface::scan(){
  ;
}

TURIPdataType turipClientInterface::getType(uint64_t id, uint8_t port){
  return TURIP_TYPE_UNKNOWN;
}

int turipClientInterface::write(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  return TURIP_ERR;
}

int turipClientInterface::read(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  return TURIP_ERR;
}
