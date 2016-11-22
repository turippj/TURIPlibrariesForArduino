#include <Arduino.h>
#include "TURIPonSPIserver.h"
using namespace ns_TURIPonSPIserver;

volatile struct st_TURIPonSPIserver_buffer ns_TURIPonSPIserver::rxBuf, ns_TURIPonSPIserver::txBuf;
cl_TURIPonSPIserver TURIPonSPIserver;

cl_TURIPonSPIserver::cl_TURIPonSPIserver(){
  // REVIEW: バッファリングサイズについては要検討。大体の用途には十分だと思うけど・・・。
  //         というかこれ以上はユーザー領域への直接アクセスとかにしないといけないかも
  rxBuf.maxLength = 32; rxBuf.readPoint = 0; rxBuf.writePoint = 0;
  txBuf.maxLength = 32; txBuf.readPoint = 0; txBuf.writePoint = 0;
  rxBuf.buffer = new uint8_t[rxBuf.maxLength];
  txBuf.buffer = new uint8_t[txBuf.maxLength];
  numof_ports = 0;
  portTable = NULL;
}

void cl_TURIPonSPIserver::begin(uint32_t model, uint32_t serial){
  cv_model = model;
  cv_serial = serial;
  add(65, &cv_model, 4);
  add(66, &cv_serial, sizeof(uint32_t));

  // Setting SPI
  DDRB = DDRB | 0B00010000;  // Set "OUTPUT" MISO
  PORTB = PORTB | 0B00011000;  // Pullup MISO,MOSI
  SPDR = 0xff;
  SPCR = _BV(SPE) | _BV(SPIE);

  // Setting SS interrupt
  PCMSK0 = _BV(PCINT1);
  PCICR = PCICR | _BV(PCIE0);

  sei();
}

int cl_TURIPonSPIserver::add(uint8_t port, void* data, size_t datasize){
  // FIXME: 同じポートに追加されたとき、既存データの更新を行いたい
  //        現状は既存データと競合してしまい、意図しない動作が発生しかねない

  if(numof_ports == 0){
    portTable = new struct st_portTable[1];
  }else{
    // REVIEW: 無駄にメモリ食いまくるけど、いいのコレ？もう少しどうにかならない？
    // portTableを拡張する
    struct st_portTable* tempTable = new struct st_portTable[numof_ports];
    // メモリ領域の確保に失敗したとき、関数がエラー(-1)を返す
    // REVIEW: メモリ確保失敗したとき本当にNULL返す？
    if(tempTable == NULL) return -1;
    for(int i=0;i<numof_ports;i++){
      tempTable[i] = portTable[i];
    }
    delete[] portTable;
    portTable = new struct st_portTable[numof_ports + 1];
    for(int i=0;i<numof_ports;i++){
      portTable[i] = tempTable[i];
    }
    delete[] tempTable;
  }

  // 拡張された領域にデータを追加する
  portTable[numof_ports].port = port;
  portTable[numof_ports].data = data;
  portTable[numof_ports].datasize = datasize;

  numof_ports++;
  return numof_ports;
}

int cl_TURIPonSPIserver::serachPort(uint8_t port){
  for(int i = 0; i < numof_ports; i++){
    // TODO: 探索アルゴリズムの改善。今は逐次比較してるだけ・・・。
    if(portTable[i].port == port) return i;
  }
  return -1;
}

void cl_TURIPonSPIserver::update(){
  // 送信処理が終わるまではその他の処理はしない
  if(txBuf.readPoint != txBuf.writePoint){
    return;
  }

  if(rxBuf.writePoint){
    // rxバッファに何かしら入っている->有効なデータの可能性あり
    // ヘッダを解析してデータが足りているか確認し、足りている場合は実際の処理を行う
    int index = serachPort(rxBuf.buffer[0] & 0x7f);

    if(index == -1){
      noInterrupts();
      //バッファのリセット
      rxBuf.writePoint = 0;
      rxBuf.readPoint = 0;
      txBuf.writePoint = 0;
      txBuf.readPoint = 0;
      // NACK送信
      txBuf.buffer[txBuf.writePoint++] = 0xA0;
      interrupts();
    }else if(rxBuf.buffer[0]&0x80){
      //受信モード(client->server)の処理
      if(rxBuf.writePoint > portTable[index].datasize){
        // データが十分に揃っている場合
        //　ユーザー領域へデータをコピー
        for(rxBuf.readPoint = 1; rxBuf.readPoint <= portTable[index].datasize; rxBuf.readPoint++){
          *((uint8_t*)portTable[index].data + rxBuf.readPoint - 1) = rxBuf.buffer[rxBuf.readPoint];
        }

        noInterrupts();
        //バッファのリセット
        rxBuf.writePoint = 0;
        rxBuf.readPoint = 0;
        txBuf.writePoint = 0;
        txBuf.readPoint = 0;
        // ACK送信
        txBuf.buffer[txBuf.writePoint++] = 0xC0;
        interrupts();
      }
    }else{
      // 送信モード(server->client)の処理
      // ユーザー領域を送信バッファへコピー

      noInterrupts();
      // バッファのリセット
      rxBuf.writePoint = 0;
      rxBuf.readPoint = 0;
      txBuf.writePoint = 0;
      txBuf.readPoint = 0;
      // 転送
      txBuf.buffer[txBuf.writePoint++] = 0xC0;
      while(txBuf.writePoint - 1 < portTable[index].datasize){
        txBuf.buffer[txBuf.writePoint] = *((uint8_t*)portTable[index].data + txBuf.writePoint - 1);
        txBuf.writePoint++;
        if(txBuf.writePoint == txBuf.maxLength) break;
      }
      interrupts();

    }
  }
}
