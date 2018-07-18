#ifndef TURIP_CLIENT_H
#define TURIP_CLIENT_H

// #include <stddef.h>
// #include <stdint.h>
// #include <string.h>
#include <Arduino.h>
#include "lib/defines.h"

#define TURIP_MAX_DEVICES 16
#define TURIP_MAX_CLIENT_INTERFACES 4

class turipClientInterface{
public:
  virtual void scan();
  virtual TURIPdataType getType(uint64_t id, uint8_t port);
  virtual int read(uint64_t id, uint8_t port, TURIPdataType type, void* data);
  virtual int write(uint64_t id, uint8_t port, TURIPdataType type, void* data);
};

typedef struct {
  uint64_t id;
  uint8_t localAddr;
  turipClientInterface* interface;
} turipDeviceAddr;

class TURIPdevice{
public:
  TURIPdevice();
  int attach(uint64_t id);
  int attach(uint32_t model, uint32_t serial){
    return attach((uint64_t)model << 32 | (uint64_t)serial);
  }
  TURIPdataType getType(uint8_t port);

  int8_t readInt8(uint8_t port);
  int16_t readInt16(uint8_t port);
  int32_t readInt32(uint8_t port);
  int64_t readInt64(uint8_t port);
  uint8_t readUint8(uint8_t port);
  uint16_t readUint16(uint8_t port);
  uint32_t readUint32(uint8_t port);
  uint64_t readUint64(uint8_t port);
  float readFloat(uint8_t port);
  double readDouble(uint8_t port);
  void readString(uint8_t port, char* data, size_t length);

  void writeInt8(uint8_t port, int8_t data);
  void writeInt16(uint8_t port, int16_t data);
  void writeInt32(uint8_t port, int32_t data);
  void writeInt64(uint8_t port, int64_t data);
  void writeUint8(uint8_t port, uint8_t data);
  void writeUint16(uint8_t port, uint16_t data);
  void writeUint32(uint8_t port, uint32_t data);
  void writeUint64(uint8_t port, uint64_t data);
  void writeFloat(uint8_t port, float data);
  void writeDouble(uint8_t port, double data);
  void writeString(uint8_t port, const char* data);

private:
  turipDeviceAddr addr;
};

class cl_TURIPclient{
public:
  cl_TURIPclient();
  void scan();
  void addTuripClientInterface(turipClientInterface* interface);
  turipDeviceAddr search(uint64_t id);

  turipDeviceAddr devices[TURIP_MAX_DEVICES];
  size_t numofDevices;

private:
  turipClientInterface* interfaces[TURIP_MAX_CLIENT_INTERFACES];
  size_t numofInterfaces;
};
extern cl_TURIPclient TURIPclient;

#endif
