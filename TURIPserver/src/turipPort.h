#ifndef TURIP_PORT_H
#define TURIP_PORT_H

// #include <stddef.h>
// #include <stdint.h>
// #include <string.h>
#include <Arduino.h>
#include "turipDefines.h"

#define MAX_STRING_LENGTH 256

/*
class TURIPport

TURIPのポートの管理を行う。
1つのインスタンス毎に1つのポートを管理する。
*/
class TURIPport{
public:
  TURIPport(uint8_t portNum);

  int8_t readInt8();
  int16_t readInt16();
  int32_t readInt32();
  int64_t readInt64();
  uint8_t readUint8();
  uint16_t readUint16();
  uint32_t readUint32();
  uint64_t readUint64();
  float readFloat();
  double readDouble();
  String readString();

  void writeInt8(int8_t data);
  void writeInt16(int16_t data);
  void writeInt32(int32_t data);
  void writeInt64(int64_t data);
  void writeUint8(uint8_t data);
  void writeUint16(uint16_t data);
  void writeUint32(uint32_t data);
  void writeUint64(uint64_t data);
  void writeFloat(float data);
  void writeDouble(double data);
  void writeString(String data);

  int receive(uint8_t* data);
  int transmit(uint8_t* data, int maxLength);

  uint8_t portNumber;
  int type;
  int permission;
  int cacheSize;
  void* volatile cache;
  void (*preReceiveFunc)(void);
  void (*postReceiveFunc)(void);
  void (*preTransmitFunc)(void);
  void (*postTransmitFunc)(void);
};

#endif
