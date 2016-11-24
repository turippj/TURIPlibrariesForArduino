#ifndef TURIP_SERVER_H
#define TURIP_SERVER_H
#include <Arduino.h>
#include "TURIPport.h"

class cl_TURIPserver{
public:
  cl_TURIPserver();
  void begin(uint32_t model, uint32_t serial);
  int add(
    uint8_t port, void* data, TURIPdataType type,
    TURIPportPermission permission);
  int add(
    uint8_t port, void* data, TURIPdataType type,
    TURIPportPermission permission,
    int (*fn_preCallInRead)(void),
    int (*fn_preCallInWrite)(void),
    int (*fn_postCallInRead)(void),
    int (*fn_postCallInWrite)(void));
  int read(uint8_t port, void* data);
  int write(uint8_t port, const void* data);
  TURIPdataType getType(uint8_t port);

private:
  cl_TURIPport** list_port;
  int numof_port;
  uint32_t model, serial;
};
extern cl_TURIPserver TURIPserver;

#endif
