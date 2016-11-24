#include "TURIPserver.h"

cl_TURIPserver TURIPserver;

cl_TURIPserver::cl_TURIPserver(){
  numof_port = 0;
  list_port = new TURIPport*[10];
}

int cl_TURIPserver::begin(uint32_t model, uint32_t serial){
  this.model = model;
  this.serial = serial;
  add(65, this->model, UINT32, READ);
  add(66, this->serial, UINT32, READ);
  return 0;
}

int cl_TURIPserver::add(
  uint8_t port, void* data, TURIPdataType type,
  TURIPportPermission permission)
{
  cl_TURIPport* newPort = new cl_TURIPport(port, data, type, permission);
  list_port[numof_port++] = newPort;
  return 0;
}

int cl_TURIPserver::add(
  uint8_t port, void* data, TURIPdataType type,
  TURIPportPermission permission,
  int (*fn_preCallInRead)(void),
  int (*fn_preCallInWrite)(void),
  int (*fn_postCallInRead)(void),
  int (*fn_postCallInWrite)(void))
{
  cl_TURIPport* newPort = new cl_TURIPport(
    port, data, type, permission,
    fn_preCallInRead, fn_preCallInWrite,
    fn_postCallInRead, fn_postCallInWrite
  );
  list_port[numof_port++] = newPort;
  return 0;
}

int cl_TURIPserver::read(uint8_t port, void* data){
  for(int i = 0; i < numof_port; i++){
    if(list_port[i]->port == port){
      return list_port[i]->read(data);
    }
  }
  return -1;
}

int cl_TURIPserver::write(uint8_t port, const void* data){
  for(int i = 0; i < numof_port; i++){
    if(list_port[i]->port == port){
      return list_port[i]->write(data);
    }
  }
  return -1;
}

TURIPdataType cl_TURIPserver::getType(uint8_t port){
  for(int i = 0; i < numof_port; i++){
    if(list_port[i]->port == port){
      return list_port[i]->type;
    }
  }
  return -1;
}

int cl_TURIPserver::isPortExist(uint8_t port){
  for(int i = 0; i < numof_port; i++){
    if(list_port[i]->port == port){
      return 0;
    }
  }
  return -1;
}

int cl_TURIPserver::getSizeofPort(uint8_t port){
  for(int i = 0; i < numof_port; i++){
    if(list_port[i]->port == port){
      return list_port[i]->getDatasize();
    }
  }
  return -1;
}
