#include "TURIPonSPIserver.h"
#include <Arduino.h>
using namespace ns_TURIPonSPIserver;

ISR(SPI_STC_vect){
  // 受信バッファが空の場合、0xff以外が来たときにデータ格納
  // 受信バッファに何か入っている場合、無条件で格納。ただし、溢れるようなら格納しない。
  if(rxBuf.writePoint == 0 && SPDR != 0xff){
    rxBuf.buffer[rxBuf.writePoint++] = SPDR;
  }else if(rxBuf.writePoint > 0 && rxBuf.writePoint < rxBuf.maxLength){
    rxBuf.buffer[rxBuf.writePoint++] = SPDR;
  }
  // 送信バッファに何かあったら送信する
  if(txBuf.readPoint < txBuf.writePoint) SPDR = txBuf.buffer[txBuf.readPoint++];
  else SPDR = 0xff;
}

ISR(PCINT0_vect){
  rxBuf.writePoint = 0;
  rxBuf.readPoint = 0;
  txBuf.writePoint = 0;
  txBuf.readPoint = 0;
  SPDR = 0xff;
}
