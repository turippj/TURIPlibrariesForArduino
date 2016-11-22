#include "TURIPclient.h"

TURIP::TURIP(uint64_t id){
  begin(id);
}

TURIP::TURIP(){
  id = 0;
  device = NULL;
}

int TURIP::begin(uint64_t id){
  this->id = id;
  for(unsigned int i = 0; i < TURIPclient.numof_device; i++){
    if(TURIPclient.device[i]->id == id){
      device = TURIPclient.device[i];
      return 0;
    }
  }
  return -1;
}

int TURIP::read(uint8_t port, void* data, unsigned int datasize){
  if(device == NULL) return -1;
  return device->read(id, port, data, datasize);
}

int TURIP::write(uint8_t port, void* data, unsigned int datasize){
  if(device == NULL) return -1;
  return device->write(id, port, data, datasize);
}
