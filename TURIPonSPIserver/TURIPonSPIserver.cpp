#include <Arduino.h>
#include "TURIPonSPIserver.h"
using namespace ns_TURIPonSPIserver;

struct st_TURIPonSPIserver_buffer ns_TURIPonSPIserver::rxBuf;
struct st_TURIPonSPIserver_buffer ns_TURIPonSPIserver::txBuf;
cl_TURIPonSPIserver TURIPonSPIserver;

cl_TURIPonSPIserver::cl_TURIPonSPIserver(){
  // REVIEW: バッファリングサイズについては要検討。大体の用途には十分だと思うけど・・・。
  //         というかこれ以上はユーザー領域への直接アクセスとかにしないといけないかも
  rxBuf.maxLength = 32; rxBuf.readPoint = 0; rxBuf.writePoint = 0;
  txBuf.maxLength = 32; txBuf.readPoint = 0; txBuf.writePoint = 0;
  rxBuf.buffer = new uint8_t[rxBuf.maxLength];
  txBuf.buffer = new uint8_t[txBuf.maxLength];
}

void cl_TURIPonSPIserver::begin(){

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

void cl_TURIPonSPIserver::update(){
  // 送信処理が終わるまではその他の処理はしない
  if(txBuf.readPoint != txBuf.writePoint){
    return;
  }

  if(rxBuf.writePoint){
    // rxバッファに何かしら入っている->有効なデータの可能性あり
    // ヘッダを解析してデータが足りているか確認し、足りている場合は実際の処理を行う
    uint8_t port = rxBuf.buffer[0] & 0x7f;
    if(TURIPserver.isPortExist(port)){
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
      //Writeモード(client->server)の処理
      int sizeof_data = TURIPserver.getSizeofPort(port); // BUG: 可変長データに対応できない
      if(rxBuf.writePoint > sizeof_data){
        // データが十分に揃っている場合
        //　ユーザー領域へデータをコピー
        TURIPserver.write(port, &rxBuf.buffer[1]);
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
      // Readモード(server->client)の処理
      // ユーザー領域を送信バッファへコピー
      noInterrupts();
      // バッファのリセット
      rxBuf.writePoint = 0;
      rxBuf.readPoint = 0;
      txBuf.writePoint = 0;
      txBuf.readPoint = 0;
      // 転送
      txBuf.buffer[txBuf.writePoint++] = 0xC0;
      TURIPserver.read(port, &txBuf.buffer[txBuf.writePoint]);
      interrupts();

    }
  }
}
