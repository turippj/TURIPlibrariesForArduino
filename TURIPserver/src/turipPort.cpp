#include "TURIPport.h"

int numofTURIPports = 0;
TURIPport** TURIPports = NULL;

void registerTURIPport(TURIPport* newPort){
  TURIPport** newTURIPports = new *TURIPport[++numofTURIPports];
  for(int i = 0; i < numofTURIPports - 1; i++){
    newTURIPports[i] = TURIPports[i];
  }
  newTURIPports[numofTURIPports - 1] = newPort;
  if(TURIPports != NULL){
    delete[] TURIPports;
  }
}

TURIPport* searchTURIPport(int portNum){
  for(int i = 0; i < numofTURIPports; i++){
    if(TURIPports[i]->portNumber == portNum){
      return TURIPports[i];
    }
  }
  return NULL;
}

TURIPport::TURIPport(int portNum, boolean data)
{
  init(portNum);
  this->type = TURIP_TYPE_BOOL;
}

TURIPport::TURIPport(int portNum, int8_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_INT8;
}

TURIPport::TURIPport(int portNum, int16_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_INT16;
}

TURIPport::TURIPport(int portNum, int32_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_INT32;
}

TURIPport::TURIPport(int portNum, int64_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_INT64;
}

TURIPport::TURIPport(int portNum, uint8_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_UINT8;
}

TURIPport::TURIPport(int portNum, uint16_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_UINT16;
}

TURIPport::TURIPport(int portNum, uint32_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_UINT32;
}

TURIPport::TURIPport(int portNum, uint64_t data)
{
  init(portNum);
  this->type = TURIP_TYPE_UINT64;
}

TURIPport::TURIPport(int portNum, float* data)
{
  init(portNum);
  this->type = TURIP_TYPE_FLOAT;
}

TURIPport::TURIPport(int portNum, double* data)
{
  init(portNum);
  this->type = TURIP_TYPE_DOUBLE;
}

TURIPport::TURIPport(int portNum, char* data)
{
  init(portNum);
  this->type = TURIP_TYPE_STRING;
}

void TURIPport::init(int portNum){
  // ポート番号の設定
  this->portNumber = portNum;

  // パーミッションのデフォルトをREADWRITEとする
  this->permission = READWRITE;

  // トリガ関数は無効化する
  this->fn_readProcess = NULL;
  this->fn_writeProcess = NULL;

  registerTURIPport(this);
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

void TURIPport::write(const int8_t data);
void TURIPport::write(const int16_t data);
void TURIPport::write(const int32_t data);
void TURIPport::write(const int64_t data);
void TURIPport::write(const uint8_t data);
void TURIPport::write(const uint16_t data);
void TURIPport::write(const uint32_t data);
void TURIPport::write(const uint64_t data);
void TURIPport::write(const float data);
void TURIPport::write(const double data);
void TURIPport::write(const char* data);

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

void TURIPport::onPostReq(const int8_t data);
void TURIPport::onPostReq(const int16_t data);
void TURIPport::onPostReq(const int32_t data);
void TURIPport::onPostReq(const int64_t data);
void TURIPport::onPostReq(const uint8_t data);
void TURIPport::onPostReq(const uint16_t data);
void TURIPport::onPostReq(const uint32_t data);
void TURIPport::onPostReq(const uint64_t data);
void TURIPport::onPostReq(const float data);
void TURIPport::onPostReq(const double data);
void TURIPport::onPostReq(const char* data);
