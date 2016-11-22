#ifndef TURIP_SERVER_H
#define TURIP_SERVER_H
#include <Arduino.h>
#include "TURIPport.h"

class cl_TURIPserver{
public:
  cl_TURIPserver();
  int init(uint32_t model, uint32_t serial);
  TURIPport* newPort(uint8_t port, void* data, size_t size);
  TURIPport* newPort(uint8_t port, char* data, size_t size);
  template <typename T> TURIPport* newPort(uint8_t port, T* data);
  TURIPport* getPort(uint8_t port);
private:
  uint32_t Model, Serial;
  int Numof_port, Sizeof_ary_p_port;
  TURIPport** Ary_p_port;

  template <typename T> void setDatatype(TURIPport* port, T* data);
};
extern cl_TURIPserver TURIPslave;

#endif
