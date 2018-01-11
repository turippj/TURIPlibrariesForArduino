#include "TURIPclientSPI.h"
#include <SPI.h>

cl_TURIPclientSPI TURIPclientSPI;

cl_TURIPclientSPI::cl_TURIPclientSPI(){
  // TURIPclient.addPeripheral(this);
  numOfSsPin = 0;
}

void cl_TURIPclientSPI::begin(){
  #ifdef ESP8266
  SPI.setFrequency(200000);
  SPI.setDataMode(SPI_MODE0);
  #else
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  #endif
  SPI.begin();
  TURIPclient.addPeripheral(this);
}

void cl_TURIPclientSPI::addPin(int ssPin){
  pinMode(ssPin, OUTPUT);
  digitalWrite(ssPin, HIGH);
  ssPinList[numOfSsPin++] = ssPin;
}

int cl_TURIPclientSPI::scan(uint64_t* idList[]){
  uint64_t id;
  uint32_t model, serial;
  numofDevices = 0;
  for(int i = 0; i < numOfSsPin; i++){
    // Serial.print("Scan Pin");
    // Serial.print(ssPinList[i]);
    // Serial.print(": ");
    if(!readByUseSS(ssPinList[i], 65, INT32, &model)){
      readByUseSS(ssPinList[i], 66, INT32, &serial);
      id = (uint64_t)model << 32 | (uint64_t)serial;
      idToSsPinList[numofDevices] = ssPinList[i];
      this->idList[numofDevices++] = id;
      // Serial.print(model, HEX);
      // Serial.print(" ");
      // Serial.print(serial, HEX);
    }
    // Serial.println();
  }
  *idList = this->idList;
  return numofDevices;
}

int cl_TURIPclientSPI::read(uint64_t id, int port, TURIPdataType type, void* data){
  // Serial.println("cl_TURIPclientSPI::read()");
  int ss = getSsPin(id);
  if(ss == -1){
    // Serial.println("No SS Pin.");
    return -1;
  }
  return readByUseSS(ss, port, type, data);
}

int cl_TURIPclientSPI::readByUseSS(int ss, int port, TURIPdataType type, void* data){
  // Serial.println("cl_TURIPclientSPI::readByUseSS()");
  uint8_t status;
  uint16_t timer;
  int datasize = sizeofTuripDataType(type);

  digitalWrite(ss, LOW);

  delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);

  // 通信安定化処理
  for(int i = 0; 1; i++){
    if(SPI.transfer(0xff) == 0xff) break;
    if(i == TURIP_ON_SPI_CLIENT_TRANS_RETRY) return -1;
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
  }

  // データ要求コマンド送信
  SPI.transfer((uint8_t)port & 0x7f);

  // スレーブ側の応答を待機
  timer = 0;
  do {
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    status = (uint8_t)SPI.transfer(0xff);
    if(++timer == TURIP_ON_SPI_CLIENT_TRANS_RETRY){
      digitalWrite(ss, HIGH);
      // Serial.println("Err 1");
      return -1;
    }
  } while (status == 0xff);
  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    // Serial.println("Err 2");
    return -2;
  }

  // データ受信
  // Serial.print("Data type: ");
  // Serial.println(type);
  // Serial.print("Data size: ");
  // Serial.println(datasize);
  // Serial.print("Data: ");
  for(unsigned int i = 0; i < datasize; i++){
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    *((char*)data + i) = SPI.transfer(0xff);
    // Serial.print(*((char*)data + i),HEX);
    // Serial.print(" ");
  }
  digitalWrite(ss, HIGH);
  // Serial.println();
  return 0;
}

int cl_TURIPclientSPI::write(uint64_t id, int port, TURIPdataType type, void* data){
  int ss = getSsPin(id);
  int datasize = sizeofTuripDataType(type);
  uint8_t status;
  uint16_t timer;

  if(ss == -1) return -1;

  digitalWrite(ss, LOW);

  delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);

  // 通信安定化処理
  for(int i = 0; 1; i++){
    if(SPI.transfer(0xff) == 0xff) break;
    if(i == TURIP_ON_SPI_CLIENT_TRANS_RETRY) return -1;
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
  }

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
  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    return -2;
  }

  digitalWrite(ss, HIGH);
  return 0;
}

TURIPdataType cl_TURIPclientSPI::getType(uint64_t id, int port){
  int ss = getSsPin(id);
  if(ss == -1){
    return UNKNOWN;
  }

  uint8_t status;
  uint16_t timer;

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
      return UNKNOWN;
    }
  } while (status == 0xff);

  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    return UNKNOWN;
  }

  digitalWrite(ss, HIGH);

  // Serial.print("DataType: ");
  // Serial.println(status & 0x0f, HEX);

  switch(status & 0x0f){
    case 0x0: return UNKNOWN;
    case 0x1: return BOOL;
    case 0x2: return INT8;
    case 0x3: return INT16;
    case 0x4: return INT32;
    case 0x5: return INT64;
    case 0xc: return FLOAT;
    case 0xd: return DOUBLE;
  }
  return UNKNOWN;
}

int cl_TURIPclientSPI::getSsPin(uint64_t id){
  // Serial.print("getSSPin: ");
  // Serial.print((uint32_t)(id>>32), HEX);
  // Serial.print(" ");
  // Serial.print((uint32_t)(id), HEX);
  // Serial.print("->");
  for(int i=0; i<numofDevices; i++){
    if(id == idList[i]){
      // Serial.println(idToSsPinList[i]);
      return idToSsPinList[i];
    }
  }
  // Serial.println("NULL");
  return -1;
}

int cl_TURIPclientSPI::isExist(uint64_t id){
  for(int i=0; i<numofDevices; i++){
    if(id == idList[i]){
      return 1;
    }
  }
  return 0;
}
