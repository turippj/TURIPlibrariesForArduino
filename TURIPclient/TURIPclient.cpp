#include "TURIPclient.h"
// #include <Arduino.h>

TURIPclientPeripheral::TURIPclientPeripheral(){
  numofDevices = 0;
}

int TURIPclientPeripheral::scan(uint64_t* idList[]){
  return -1;
}

int TURIPclientPeripheral::write(uint64_t id, int port, TURIPdataType type, void* data){
  return -1;
}

int TURIPclientPeripheral::read(uint64_t id, int port, TURIPdataType type, void* data){
  return -1;
}




cl_TURIPclient TURIPclient;

cl_TURIPclient::cl_TURIPclient(){
  numofPeripherals = 0;
  numofDevices = 0;
}

int cl_TURIPclient::addPeripheral(TURIPclientPeripheral* peripheral){
  peripheralList[numofPeripherals++] = peripheral;
  return 0;
}

TURIPclientPeripheral* cl_TURIPclient::getPeripheral(uint64_t id){
  for(int i=0; i<numofPeripherals; i++){
    uint64_t* idListInPeripheral;
    int n = peripheralList[i]->scan(&idListInPeripheral);
    for(int j=0; j<n; j++){
      if(idListInPeripheral[j] == id) return peripheralList[i];
    }
  }
  return NULL;
}

int cl_TURIPclient::scan(){
  numofDevices = 0;
  for(int i=0; i<numofPeripherals; i++){
    uint64_t* idListInPeripheral;
    int n = peripheralList[i]->scan(&idListInPeripheral);
    for(int j=0; j<n; j++){
      idList[numofDevices + j] = idListInPeripheral[j];
    }
    numofDevices += n;
  }
  return numofDevices;
}

int cl_TURIPclient::scan(uint64_t* idList[]){
  scan();
  *idList = this->idList;
  return numofDevices;
}

int cl_TURIPclient::isExist(uint64_t id){
  for(int i=0; i<numofDevices; i++){
    if(idList[i] == id) return 0;
  }
  return -1;
}




TURIP::TURIP(){
  id = 0;
  peripheral = NULL;
}

int TURIP::attach(uint64_t id){
  peripheral = TURIPclient.getPeripheral(id);
  if(peripheral == NULL) return -1;
  this->id = id;
  // Serial.println("attached!");
  return 0;
}

int TURIP::read(int port, TURIPdataType type, void* data){
  if(peripheral == NULL) return -1;
  return peripheral->read(id, port, type, data);
}

int TURIP::write(int port, TURIPdataType type, void* data){
  if(peripheral == NULL) return -1;
  return peripheral->write(id, port, type, data);
}
