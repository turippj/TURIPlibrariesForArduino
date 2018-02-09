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
  SPI.begin();
  #elif ESP32
  SPI.setFrequency(200000);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin(17, 18, 16, 19); // Temporal setting for test device.
  #else
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.begin();
  #endif
  TURIPclient.addTuripClientInterface(this);
}

void cl_TURIPclientSPI::addPin(int ssPin){
  pinMode(ssPin, OUTPUT);
  digitalWrite(ssPin, HIGH);
  ssPinList[numOfSsPin++] = ssPin;
}

void cl_TURIPclientSPI::scan(){
  uint64_t id;
  uint32_t model, serial;
  for(int i = 0; i < numOfSsPin; i++){
    if(!readByUseSS(ssPinList[i], TURIP_PORT_MODEL, TURIP_TYPE_INT32, &model)){
      readByUseSS(ssPinList[i], TURIP_PORT_SERIAL, TURIP_TYPE_INT32, &serial);
      id = (uint64_t)model << 32 | (uint64_t)serial;
      TURIPclient.devices[TURIPclient.numofDevices].id = id;
      TURIPclient.devices[TURIPclient.numofDevices].interface = this;
      TURIPclient.devices[TURIPclient.numofDevices].localAddr = ssPinList[i];
      TURIPclient.numofDevices++;
    }
  }
}

int cl_TURIPclientSPI::read(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  int ss = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      ss = TURIPclient.devices[i].localAddr;
    }
  }
  if(ss == -1){
    return TURIP_ERR;
  }
  return readByUseSS(ss, port, type, data);
}

int cl_TURIPclientSPI::readByUseSS(int ss, uint8_t port, TURIPdataType type, void* data){
  // Serial.println("cl_TURIPclientSPI::readByUseSS()");
  uint8_t status;
  uint16_t timer;
  int datasize;
  switch (type) {
    case TURIP_TYPE_BOOL:
    case TURIP_TYPE_INT8:
    case TURIP_TYPE_UINT8:
      datasize = 1;
      break;
    case TURIP_TYPE_INT16:
    case TURIP_TYPE_UINT16:
      datasize = 2;
      break;
    case TURIP_TYPE_INT32:
    case TURIP_TYPE_UINT32:
    case TURIP_TYPE_FLOAT:
      datasize = 4;
      break;
    case TURIP_TYPE_INT64:
    case TURIP_TYPE_UINT64:
    case TURIP_TYPE_DOUBLE:
      datasize = 8;
      break;
    default:
      datasize = 0;
  }

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
      return TURIP_ERR;
    }
  } while (status == 0xff);
  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    return TURIP_ERR;
  }

  for(unsigned int i = 0; i < datasize; i++){
    delayMicroseconds(TURIP_ON_SPI_CLIENT_TRANS_DELAY);
    *((char*)data + i) = SPI.transfer(0xff);
  }
  digitalWrite(ss, HIGH);
  return TURIP_OK;
}

int cl_TURIPclientSPI::write(uint64_t id, uint8_t port, TURIPdataType type, void* data){
  int ss = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      ss = TURIPclient.devices[i].localAddr;
    }
  }
  if(ss == -1){
    return TURIP_ERR;
  }
  int datasize;
  uint8_t status;
  uint16_t timer;

  switch (type) {
    case TURIP_TYPE_BOOL:
    case TURIP_TYPE_INT8:
    case TURIP_TYPE_UINT8:
      datasize = 1;
      break;
    case TURIP_TYPE_INT16:
    case TURIP_TYPE_UINT16:
      datasize = 2;
      break;
    case TURIP_TYPE_INT32:
    case TURIP_TYPE_UINT32:
    case TURIP_TYPE_FLOAT:
      datasize = 4;
      break;
    case TURIP_TYPE_INT64:
    case TURIP_TYPE_UINT64:
    case TURIP_TYPE_DOUBLE:
      datasize = 8;
      break;
    default:
      datasize = 0;
  }

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
      return TURIP_ERR;
    }
  } while (status == 0xff);
  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    return TURIP_ERR;
  }

  digitalWrite(ss, HIGH);
  return TURIP_OK;
}

TURIPdataType cl_TURIPclientSPI::getType(uint64_t id, uint8_t port){
  int ss = -1;
  for(int i = 0; i < TURIPclient.numofDevices; i++){
    if(TURIPclient.devices[i].id == id){
      ss = TURIPclient.devices[i].localAddr;
    }
  }
  if(ss == -1){
    return TURIP_TYPE_UNKNOWN;
  }

  uint8_t status;
  uint16_t timer;

  digitalWrite(ss, LOW);

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
      return TURIP_TYPE_UNKNOWN;
    }
  } while (status == 0xff);

  if((status & 0xf0) != 0x00) {
    digitalWrite(ss, HIGH);
    return TURIP_TYPE_UNKNOWN;
  }

  digitalWrite(ss, HIGH);

  return (TURIPdataType)(status & 0x0f);
}
