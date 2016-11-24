#include "TURIPserver.h"

cl_TURIPport::cl_TURIPport(
  uint8_t port, void* data, TURIPdataType type,
  TURIPportPermission permission)
{
  this->port = port;
  this->data = data;
  this->type = type;
  this->permission = permission;
  this->fn_preCallInRead = NULL;
  this->fn_preCallInWrite = NULL;
  this->fn_postCallInRead = NULL;
  this->fn_postCallInWrite = NULL;
}

cl_TURIPport::cl_TURIPport(
  uint8_t port, void* data, TURIPdataType type,
  TURIPportPermission permission,
  int (*fn_preCallInRead)(void),
  int (*fn_preCallInWrite)(void),
  int (*fn_postCallInRead)(void),
  int (*fn_postCallInWrite)(void))
{
  this->port = port;
  this->data = data;
  this->type = type;
  this->permission = permission;
  this->fn_preCallInRead = fn_preCallInRead;
  this->fn_preCallInWrite = fn_preCallInWrite;
  this->fn_postCallInRead = fn_postCallInRead;
  this->fn_postCallInWrite = fn_postCallInWrite;
}

int cl_TURIPport::read(void** data)
{
  if(this->permission != READ && this->permission != READWRITE){
    return -1;
  }
  // memcpy(data, this->data, getDatasize(data));
  data = this->data;
  return 0;
}

int cl_TURIPport::write(const void* data)
{
  if(this->permission != WRITE && this->permission != READWRITE){
    return -1;
  }
  memcpy(this->data, data, getDatasize(data));
  return 0;
}

int cl_TURIPport::getDatasize(const void* data){
  switch(this->type){
    case INT8:
    case UINT8:
      return 1;
    case INT16:
    case UINT16:
      return 2;
    case INT32:
    case UINT32:
    case FLOAT32:
      return 4;
    case INT64:
    case UINT64:
    case FLOAT64:
      return 8;
    case STRING:
      for(int i=0;;i++){
        if((const char*)((const char*)data + i) == '\0'){
          return i+1;
        }
      }
    default:
      return -1;
  }
}
