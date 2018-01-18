#ifndef TURIP_PORT_H
#define TURIP_PORT_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "turipDefines.h"

/*
class TURIPport

TURIPのポートの管理を行う。
1つのインスタンス毎に1つのポートを管理する。
*/
class TURIPport{
public:
  TURIPport(int portNum);

  int8_t TURIPport::readInt8();
  int16_t TURIPport::readInt16();
  int32_t TURIPport::readInt32();
  int64_t TURIPport::readInt64();
  uint8_t TURIPport::readInt8();
  uint16_t TURIPport::readUint16();
  uint32_t TURIPport::readUint32();
  uint64_t TURIPport::readUint64();
  float TURIPport::readFloat();
  double TURIPport::readDouble();
  int TURIPport::readString(char* data, unsigned int maxLength);

  void TURIPport::writeInt8(int8_t data);
  void TURIPport::writeInt16(int16_t data);
  void TURIPport::writeInt32(int32_t data);
  void TURIPport::writeInt64(int64_t data);
  void TURIPport::writeUint8(uint8_t data);
  void TURIPport::writeUint16(uint16_t data);
  void TURIPport::writeUint32(uint32_t data);
  void TURIPport::writeUint64(uint64_t data);
  void TURIPport::writeFloat(float data);
  void TURIPport::writeDouble(double data);
  void TURIPport::writeString(char* data);

  int TURIPport::receive(int type, uint8_t* dataPtr, unsigned int nbytes);
  int TURIPport::transmit(uint8_t* dataPtr);

  uint8_t portNumber;
  int type;
  turipPortPermission permission;
  void* volatile cache;
  void (*preReceiveFunc)(void);
  void (*postReceiveFunc)(void);
  void (*preTransmitFunc)(void);
  void (*postTransmitFunc)(void);
};

#endif
