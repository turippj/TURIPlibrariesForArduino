#include "TURIPclient.h"

cl_TURIPclient TURIPclient;

/*
## デバッグ用関数
IDをシリアル出力するよ
*/
void printId(uint64_t id){
  union un_id{
    uint64_t id64;
    byte id8[8];
  } idConv;
  idConv.id64 = id;
  for(int i = 7; i >= 0; i--){
    Serial.print(idConv.id8[i], HEX);
    if(i) Serial.print(" ");
  }
}

/*
## cl_TURIPdevice
*/
cl_TURIPdevice::cl_TURIPdevice(){
  read = NULL;
  write = NULL;
}

cl_TURIPdevice::cl_TURIPdevice(uint64_t id){
  this->id = id;
  read = NULL;
  write = NULL;
}

/*
## cl_TURIPperipheral
*/
cl_TURIPperipheral::cl_TURIPperipheral(){
  numof_device = 0;
  *device = NULL;
  scan = NULL;
}

/*
## cl_TURIPclient
*/
cl_TURIPclient::cl_TURIPclient(){
  numof_device = 0;
  numof_peripheral = 0;
  sizeof_deviceList = 10;
  sizeof_peripheralList = 3;
  device = new cl_TURIPdevice*[sizeof_deviceList];
  peripheral = new cl_TURIPperipheral*[sizeof_peripheralList];
}

int cl_TURIPclient::scan(){
  int numof_device = 0;
  for(unsigned int i = 0; i < numof_peripheral; i++){
    peripheral[i]->scan();
    for(int j = 0; j < peripheral[i]->numof_device; j++){
      device[numof_device++] = peripheral[i]->device[j];
    }
  }
  this->numof_device = numof_device;
  return numof_device;
}

int cl_TURIPclient::addPeripheral(cl_TURIPperipheral* peripheral){
  this->peripheral[numof_peripheral++] = peripheral;
  return numof_peripheral;
}

int cl_TURIPclient::addDevice(cl_TURIPdevice* device){
  this->device[numof_device++] = device;
  return numof_device;
}
