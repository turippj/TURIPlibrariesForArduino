#include "TURIPserver.h"

TURIPport::TURIPport(uint8_t port, void* data, size_t dataSize){
  this->port = port;
  this->data = data;
  this->sizeof_data = dataSize;
  this->type = BINARY;
  this->permission = READWRITE;
}

int TURIPport::read(void* data){
  if(this->permission == WRITE) return -1;
  memcpy(data, this->data, this->sizeof_data);
  return this->sizeof_data;
}

int TURIPport::write(const void* data){
  if(this->permission == READ) return -1;
  memcpy(this->data, data, this->sizeof_data);
  return this->sizeof_data;
}
