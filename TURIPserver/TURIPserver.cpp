#include "TURIPserver.h"

cl_TURIPserver TURIPserver;

cl_TURIPserver::cl_TURIPserver(){
  Numof_port = 0;
  Sizeof_ary_p_port = 10;
  Ary_p_port = new TURIPport*[Sizeof_ary_p_port];
}

int cl_TURIPserver::init(uint32_t model, uint32_t serial){
  Model = model;
  Serial = serial;
  if(newPort(65, &Model) == NULL) return -1;
  if(newPort(66, &Serial) == NULL) return -1;
  return 0;
}

TURIPport* cl_TURIPserver::getPort(uint8_t port){
  for(int i = 0; i < Numof_port; i++){
    if(Ary_p_port[i]->port == port) return Ary_p_port[i];
  }
  return NULL;
}

TURIPport* cl_TURIPserver::newPort(uint8_t port, char* data, size_t size){
  if(Sizeof_ary_p_port == Numof_port) return NULL;
  TURIPport* newPort = new TURIPport(port, data, size);
  newPort->type = TEXT;
  Ary_p_port[Numof_port++] = newPort;
  return newPort;
}

TURIPport* cl_TURIPserver::newPort(uint8_t port, void* data, size_t size){
  if(Sizeof_ary_p_port == Numof_port) return NULL;
  TURIPport* newPort = new TURIPport(port, data, size);
  newPort->type = BINARY;
  Ary_p_port[Numof_port++] = newPort;
  return newPort;
}

template <typename T> TURIPport* cl_TURIPserver::newPort(uint8_t port, T* data){
  if(Sizeof_ary_p_port == Numof_port) return NULL;
  TURIPport* newPort = new TURIPport(port, data, sizeof(T));
  setDatatype(newPort, data);
  Ary_p_port[Numof_port++] = newPort;
  return newPort;
}
template TURIPport* cl_TURIPserver::newPort<int8_t>(uint8_t port, int8_t* data);
template TURIPport* cl_TURIPserver::newPort<uint8_t>(uint8_t port, uint8_t* data);
template TURIPport* cl_TURIPserver::newPort<int16_t>(uint8_t port, int16_t* data);
template TURIPport* cl_TURIPserver::newPort<uint16_t>(uint8_t port, uint16_t* data);
template TURIPport* cl_TURIPserver::newPort<int32_t>(uint8_t port, int32_t* data);
template TURIPport* cl_TURIPserver::newPort<uint32_t>(uint8_t port, uint32_t* data);
template TURIPport* cl_TURIPserver::newPort<int64_t>(uint8_t port, int64_t* data);
template TURIPport* cl_TURIPserver::newPort<uint64_t>(uint8_t port, uint64_t* data);
template TURIPport* cl_TURIPserver::newPort<float>(uint8_t port, float* data);

template <typename T> void cl_TURIPserver::setDatatype(TURIPport* port, T* data){
  port->type = BINARY;
}
template <> void cl_TURIPserver::setDatatype<int8_t>(TURIPport* port, int8_t* data){
  port->type = INT8;
}
template <> void cl_TURIPserver::setDatatype<int16_t>(TURIPport* port, int16_t* data){
  port->type = INT16;
}
template <> void cl_TURIPserver::setDatatype<int32_t>(TURIPport* port, int32_t* data){
  port->type = INT32;
}
template <> void cl_TURIPserver::setDatatype<int64_t>(TURIPport* port, int64_t* data){
  port->type = INT64;
}
template <> void cl_TURIPserver::setDatatype<uint8_t>(TURIPport* port, uint8_t* data){
  port->type = UINT8;
}
template <> void cl_TURIPserver::setDatatype<uint16_t>(TURIPport* port, uint16_t* data){
  port->type = UINT16;
}
template <> void cl_TURIPserver::setDatatype<uint32_t>(TURIPport* port, uint32_t* data){
  port->type = UINT32;
}
template <> void cl_TURIPserver::setDatatype<uint64_t>(TURIPport* port, uint64_t* data){
  port->type = UINT64;
}
template <> void cl_TURIPserver::setDatatype<float>(TURIPport* port, float* data){
  port->type = FLOAT32;
}
template <> void cl_TURIPserver::setDatatype<double>(TURIPport* port, double* data){
  port->type = FLOAT64;
}
