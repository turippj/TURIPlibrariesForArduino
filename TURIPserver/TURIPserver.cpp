#include "TURIPserver.h"

TURIPport::TURIPport(int portNum, TURIPdataType type)
{
  // ポート番号の設定
  this->portNumber = portNum;

  // TURIPデータ型の設定
  this->type = type;

  // データ保管領域の確保
  // if(type == STRING){
  //   this->data = new uint8_t[1];
  //   this->data[0] = '\0';
  //   this->stringLength = 0;
  // }else{
  //   this->data = new uint8_t[sizeofTuripDataType(this->type)];
  // }

  // パーミッションのデフォルトをREADWRITEとする
  this->permission = READWRITE;

  // トリガ関数は無効化する
  this->fn_readProcess = NULL;
  this->fn_writeProcess = NULL;

  // TURIPserverへ自身を登録する
  TURIPserver.add(this);
}

void* TURIPport::readMemory(){
  if(this->fn_readProcess != NULL){
    if(this->fn_readProcess()){
      return NULL;
    }
  }
  return this->data;
}

// char* TURIPport::readStrMemory(){
//   if(this->fn_readProcess != NULL){
//     if(this->fn_readProcess()){
//       return NULL;
//     }
//   }
//   return this->data;
// }

int TURIPport::writeMemory(void* data){
  if(this->permission == READWRITE){
    memcpy(this->data, data, sizeofTuripDataType(this->type));
    if(this->fn_writeProcess == NULL){
      return 0;
    } else{
      return this->fn_writeProcess();
    }
  }else{
    return -1;
  }
}

// int TURIPport::writeStrMemory(const char* data){
//   if(this->permission == READWRITE && this->stringLength >= strlen(data)){
//     strcpy(this->data, data);
//     if(this->fn_writeProcess == NULL){
//       return 0;
//     } else{
//       return this->fn_writeProcess();
//     }
//   }else{
//     return -1;
//   }
// }

void TURIPport::setReadProcess(int (*function)()){
  this->fn_readProcess = function;
}

void TURIPport::setWriteProcess(int (*function)()){
  this->fn_writeProcess = function;
}

TURIPdataType TURIPport::getType(){
  return this->type;
}

void TURIPport::setType(TURIPdataType newType){
  delete[] this->data;
  this->type = newType;
  this->data = new uint8_t[sizeofTuripDataType(newType)];
}

// void TURIPport::setStrlen(unsigned int length){
//   delete[] this->data;
//   this->stringLength = length;
//   this->data = new uint8_t[length + 1];
//   this->data[length] = '\0';
// }




cl_TURIPserver TURIPserver;

cl_TURIPserver::cl_TURIPserver(){
  numofPorts = 0;
}

void cl_TURIPserver::begin(uint32_t model, uint32_t serial){
  portOfModel = new TURIPport(65, INT32);
  portOfSerial = new TURIPport(66, INT32);
  portOfModel->write(model);
  portOfSerial->write(serial);
  add(portOfModel);
  add(portOfSerial);
}

int cl_TURIPserver::add(TURIPport* newPort){
  if(numofPorts < 16){
    portList[numofPorts++] = newPort;
    return 0;
  }else{
    return -1;
  }
}

TURIPport* cl_TURIPserver::get(uint8_t port){
  for(int i=0;i<numofPorts;i++){
    if(portList[i]->portNumber == port){
      return portList[i];
    }
  }
  return NULL;
}
