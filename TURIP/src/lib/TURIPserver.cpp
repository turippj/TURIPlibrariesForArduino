#include "TURIPserver.h"

cl_TURIPserver TURIPserver;

cl_TURIPserver::cl_TURIPserver(){
  numofPorts = 0;
}

void cl_TURIPserver::begin(uint32_t model, uint32_t serial){
  numofPorts = 0;
  portList[numofPorts] = new TURIPport(TURIP_PORT_MODEL);
  portList[numofPorts++]->writeUint32(model);
  portList[numofPorts] = new TURIPport(TURIP_PORT_SERIAL);
  portList[numofPorts++]->writeUint32(serial);

  myId = ((uint64_t)model << 32) | (uint64_t)serial;
}

TURIPport* cl_TURIPserver::newPort(uint8_t portNumber){
  if(numofPorts < MAX_NUMOF_PORTS){
    portList[numofPorts++] = new TURIPport(portNumber);
    return portList[numofPorts - 1];
  }else{
    return NULL;
  }
}

TURIPport* cl_TURIPserver::getPort(uint8_t portNumber){
  for(int i = 0; i < numofPorts; i++){
    if(portList[i]->portNumber == portNumber){
      return portList[i];
    }
  }
  return NULL;
}
