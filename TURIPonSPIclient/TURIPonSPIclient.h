#ifndef TURIP_ON_SPI_CLIENT_H
#define TURIP_ON_SPI_CLIENT_H
#include <Arduino.h>
#include "TURIPclient.h"

#define TURIP_ON_SPI_CLIENT_TRANS_DELAY 100
#define TURIP_ON_SPI_CLIENT_TRANS_RETRY 100

class cl_TURIPonSPIclient{
public:
  cl_TURIPonSPIclient();
  void begin();
  void addSS(int ss);
};
extern cl_TURIPonSPIclient TURIPonSPIclient;

namespace ns_TURIPonSPIclient{
  struct st_idTable{
    uint64_t id;
    int ss;
  };

  int scan();
  int scan(uint64_t** idList);
  int read(uint64_t id, uint8_t port, void* data, size_t datasize);
  int write(uint64_t id, uint8_t port, void* data, size_t datasize);
  int spiRead(int ss, uint8_t port, void* data, size_t datasize);
  int spiWrite(int ss, uint8_t port, void* data, size_t datasize);
  int getSSById(uint64_t id);

  extern int c_numofSS;
  extern int c_numofId;
  extern int* c_ssList;
  extern struct st_idTable* c_idTable;
  extern cl_TURIPperipheral peripheral;
};

#endif
