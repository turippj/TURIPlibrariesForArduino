#ifndef TURIP_PORT_H
#define TURIP_PORT_H

#include "turipDefines.h"

typedef enum en_turipPortPermission{
  READONLY, READWRITE
} turipPortPermission;

/*
class TURIPport

TURIPのポートの管理を行う。
1つのインスタンス毎に1つのポートを管理する。
*/
class TURIPport{
public:
  TURIPport(int portNum, boolean data);
  TURIPport(int portNum, int8_t data);
  TURIPport(int portNum, int16_t data);
  TURIPport(int portNum, int32_t data);
  TURIPport(int portNum, int64_t data);
  TURIPport(int portNum, uint8_t data);
  TURIPport(int portNum, uint16_t data);
  TURIPport(int portNum, uint32_t data);
  TURIPport(int portNum, uint64_t data);
  TURIPport(int portNum, float* data);
  TURIPport(int portNum, double* data);
  TURIPport(int portNum, char* data);

  init(int portNum){
    // ポート番号の設定
    this->portNumber = portNum;

    // パーミッションのデフォルトをREADWRITEとする
    this->permission = READWRITE;

    // トリガ関数は無効化する
    this->fn_readProcess = NULL;
    this->fn_writeProcess = NULL;

    registerTuripPort(this);
  }

  void TURIPport::read(int8_t* data);
  void TURIPport::read(int16_t* data);
  void TURIPport::read(int32_t* data);
  void TURIPport::read(int64_t* data);
  void TURIPport::read(uint8_t* data);
  void TURIPport::read(uint16_t* data);
  void TURIPport::read(uint32_t* data);
  void TURIPport::read(uint64_t* data);
  void TURIPport::read(float* data);
  void TURIPport::read(double* data);
  void TURIPport::read(char* data);

  void TURIPport::write(int8_t data);
  void TURIPport::write(int16_t data);
  void TURIPport::write(int32_t data);
  void TURIPport::write(int64_t data);
  void TURIPport::write(uint8_t data);
  void TURIPport::write(uint16_t data);
  void TURIPport::write(uint32_t data);
  void TURIPport::write(uint64_t data);
  void TURIPport::write(float data);
  void TURIPport::write(double data);
  void TURIPport::write(char* data);

  void TURIPport::onGetReq(int8_t* data);
  void TURIPport::onGetReq(int16_t* data);
  void TURIPport::onGetReq(int32_t* data);
  void TURIPport::onGetReq(int64_t* data);
  void TURIPport::onGetReq(uint8_t* data);
  void TURIPport::onGetReq(uint16_t* data);
  void TURIPport::onGetReq(uint32_t* data);
  void TURIPport::onGetReq(uint64_t* data);
  void TURIPport::onGetReq(float* data);
  void TURIPport::onGetReq(double* data);
  void TURIPport::onGetReq(char* data);

  void TURIPport::onPostReq(int8_t data);
  void TURIPport::onPostReq(int16_t data);
  void TURIPport::onPostReq(int32_t data);
  void TURIPport::onPostReq(int64_t data);
  void TURIPport::onPostReq(uint8_t data);
  void TURIPport::onPostReq(uint16_t data);
  void TURIPport::onPostReq(uint32_t data);
  void TURIPport::onPostReq(uint64_t data);
  void TURIPport::onPostReq(float data);
  void TURIPport::onPostReq(double data);
  void TURIPport::onPostReq(char* data);

  int portNumber;
  int type;
  turipPortPermission permission;
  void (*preReadFunction)(void);
  void (*postReadFunction)(void);
  void (*preWriteFunction)(void);
  void (*postWriteFunction)(void);

private:
  void* volatile cache;
};

#endif
