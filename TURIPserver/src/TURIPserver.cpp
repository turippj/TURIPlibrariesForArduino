#include "TURIPserver.h"

cl_TURIPserver TURIPserver;

cl_TURIPserver::cl_TURIPserver(){
  numofPorts = 0;
}

void cl_TURIPserver::begin(uint32_t model, uint32_t serial){
  portOfModel = new TURIPport(65, INT32);
  portOfSerial = new TURIPport(66, INT32);
  portOfModel->write(model);
  portOfSerial->write(serial);
  add(portOfModel);
  add(portOfSerial);
}

int cl_TURIPserver::add(TURIPport* newPort){
  if(numofPorts < 16){
    portList[numofPorts++] = newPort;
    return 0;
  }else{
    return -1;
  }
}

TURIPport* cl_TURIPserver::get(uint8_t port){
  for(int i=0;i<numofPorts;i++){
    if(portList[i]->portNumber == port){
      return portList[i];
    }
  }
  return NULL;
}
