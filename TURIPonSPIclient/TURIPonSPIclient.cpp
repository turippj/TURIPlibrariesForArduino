#include "TURIPonSPIclient.h"
#include <SPI.h>

using namespace ns_TURIPonSPIclient;

cl_TURIPonSPIclient TURIPonSPIclient;
int ns_TURIPonSPIclient::c_numofSS;
int ns_TURIPonSPIclient::c_numofId;
int* ns_TURIPonSPIclient::c_ssList;
struct st_idTable* ns_TURIPonSPIclient::c_idTable;
cl_TURIPperipheral ns_TURIPonSPIclient::peripheral;

cl_TURIPonSPIclient::cl_TURIPonSPIclient(){
  c_numofSS = 0;
  c_numofId = 0;
  c_idTable = NULL;
  c_ssList = NULL;
}

void cl_TURIPonSPIclient::begin(){
  peripheral.scan = scan;
  TURIPclient.addPeripheral(&peripheral);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.begin();
}

void cl_TURIPonSPIclient::addSS(int ss){
  pinMode(ss, OUTPUT);
  digitalWrite(ss, HIGH);
  int* temp = new int[++c_numofSS];
  for(int i = 0; i < c_numofSS - 1; i++){
    temp[i] = c_ssList[i];
  }
  temp[c_numofSS - 1] = ss;
  if(c_ssList != NULL) delete c_ssList;
  c_ssList = temp;
}

int ns_TURIPonSPIclient::scan(){
  uint32_t model, serial;
  c_numofId = 0;
  for(int i = 0; i < c_numofSS; i++){
    if(!spiRead(c_ssList[i], 65, &model, sizeof(uint32_t))){
      spiRead(c_ssList[i], 66, &serial, sizeof(uint32_t));
      c_numofId++;
      struct st_idTable* temp = new struct st_idTable[c_numofId];
      temp[c_numofId - 1].ss = c_ssList[i];
      temp[c_numofId - 1].id = (uint64_t)model << 32 | (uint64_t)serial;

      if(c_idTable != NULL) delete[] c_idTable;
      c_idTable = new struct st_idTable[c_numofId];
      for(int i = 0; i < c_numofId; i++){
        c_idTable[i] = temp[i];
      }
      delete[] temp;
    }
  }

  cl_TURIPdevice* device = new cl_TURIPdevice[c_numofId];
  for(int i = 0; i < c_numofId; i++){
    device[i].id = c_idTable[i].id;
    device[i].read = read;
    device[i].write = write;
  }
  peripheral.device = &device;
  peripheral.numof_device = c_numofId;

  return c_numofId;
}

int ns_TURIPonSPIclient::scan(uint64_t** idList){
  scan();
  if(c_numofId > 0){
    if(idList != NULL) delete[] idList;
    *idList = new uint64_t[c_numofId];
    for(int i = 0; i < c_numofId; i++){
      *idList[i] = c_idTable[i].id;
    }
  }
  return c_numofId;
}

int ns_TURIPonSPIclient::getSSById(uint64_t id){
  for(int i = 0; i < c_numofId; i++){
    if(id == c_idTable[i].id) return c_idTable[i].ss;
  }
  // idが登録されていないならscan()を実行してから再検索する
  scan();
  for(int i = 0; i < c_numofId; i++){
    if(id == c_idTable[i].id) return c_idTable[i].ss;
  }
  return -1;
}

int ns_TURIPonSPIclient::read(uint64_t id, uint8_t port, void* data, size_t datasize){
  int ss = getSSById(id);
  if(ss != -1){
    return spiRead(ss, port, data, datasize);
  }else{
    return -1;
  }
}

int ns_TURIPonSPIclient::write(uint64_t id, uint8_t port, void* data, size_t datasize){
  int ss = getSSById(id);
  if(ss != -1){
    return spiWrite(getSSById(id), port, data, datasize);
  }else{
    return -1;
  }
}

int ns_TURIPonSPIclient::spiRead(int ss, uint8_t port, void* data, size_t datasize){
  uint8_t status;
  uint8_t timer;

  digitalWrite(ss, LOW);

  // 通信安定化処理
  // for(int i = 0; 1; i++){
  //   if(SPI.transfer(0xff) == 0xff) break;
  //   if(i == TURIP_ON_SPI_CLIENT_TRANS_RETRY) return -1;
  //   delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
  // }
  delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);

  // データ要求コマンド送信
  SPI.transfer((uint8_t)port & 0x7f);

  // スレーブ側の応答を待機
  timer = 0;
  do {
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    status = (uint8_t)SPI.transfer(0xff);
    if(++timer == TURIP_ON_SPI_CLIENT_TRANS_RETRY){
      digitalWrite(ss, HIGH);
      return -1;
    }
  } while (status == 0xff);
  if(status != 0xC0) {
    digitalWrite(ss, HIGH);
    return -2;
  }

  // データ受信
  for(unsigned int i = 0; i < datasize; i++){
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    *((char*)data + i) = SPI.transfer(0xff);
  }
  digitalWrite(ss, HIGH);
  return 0;
}

int ns_TURIPonSPIclient::spiWrite(int ss, uint8_t port, void* data, size_t datasize){
  uint8_t status;
  uint8_t timer;

  digitalWrite(ss, LOW);

  // 通信安定化処理
  // for(int i = 0; 1; i++){
  //   if(SPI.transfer(0xff) == 0xff) break;
  //   if(i == TURIP_ON_SPI_CLIENT_TRANS_RETRY) return -1;
  //   delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
  // }
  delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);

  // コマンド送信
  SPI.transfer((uint8_t)port | 0x80);

  // データ送信
  for(unsigned int i = 0; i < datasize; i++){
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    SPI.transfer(*((char*)data + i));
  }

  // スレーブ側の応答を待機
  timer = 0;
  do {
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    status = (uint8_t)SPI.transfer(0xff);
    if(++timer == TURIP_ON_SPI_CLIENT_TRANS_RETRY){
      digitalWrite(ss, HIGH);
      return -1;
    }
  } while (status == 0xff);
  if(status != 0x80) {
    digitalWrite(ss, HIGH);
    return -2;
  }

  digitalWrite(ss, HIGH);
  return 0;
}
