#include "TURIPserverSPI.h"

using namespace ns_TURIPserverSPI;

struct st_TURIPserverSPI_buffer ns_TURIPserverSPI::rxBuf;
struct st_TURIPserverSPI_buffer ns_TURIPserverSPI::txBuf;
cl_TURIPserverSPI TURIPserverSPI;

cl_TURIPserverSPI::cl_TURIPserverSPI(){
  rxBuf.maxLength = 16; rxBuf.readPoint = 0; rxBuf.writePoint = 0;
  txBuf.maxLength = 16; txBuf.readPoint = 0; txBuf.writePoint = 0;
  rxBuf.buffer = new uint8_t[rxBuf.maxLength];
  txBuf.buffer = new uint8_t[txBuf.maxLength];
}

void cl_TURIPserverSPI::begin(){

  // Setting SPI
  DDRB = DDRB | 0B00010000;  // Set "OUTPUT" MISO
  PORTB = PORTB | 0B00011000;  // Pullup MISO,MOSI
  SPDR = 0xff;
  SPCR = _BV(SPE) | _BV(SPIE);

  // Setting SS interrupt
  // PCMSK0 = _BV(PCINT1);
  // PCICR = PCICR | _BV(PCIE0);

  sei();
}

void cl_TURIPserverSPI::update(){
  // If SPI is desabled, buffers are reseted.
  if(digitalRead(TURIPSERVER_PIN_SS) == HIGH){
    rxBuf.writePoint = 0;
    rxBuf.readPoint = 0;
    txBuf.writePoint = 0;
    txBuf.readPoint = 0;
    SPDR = 0xff;
    return;
  }

  // 送信処理が終わるまではその他の処理はしない
  if(txBuf.readPoint != txBuf.writePoint){
    return;
  }

  if(rxBuf.writePoint){
    // rxバッファに何かしら入っている->有効なデータの可能性あり
    // ヘッダを解析してデータが足りているか確認し、足りている場合は実際の処理を行う
    TURIPport* port = TURIPserver.get(rxBuf.buffer[0] & 0x7f);
    if(port == NULL){
      noInterrupts();
      //バッファのリセット
      rxBuf.writePoint = 0;
      rxBuf.readPoint = 0;
      txBuf.writePoint = 0;
      txBuf.readPoint = 0;
      // NACK送信
      txBuf.buffer[txBuf.writePoint++] = 0x40;
      interrupts();
    }else if(rxBuf.buffer[0]&0x80){
      //Writeモード(client->server)の処理
      int sizeofData = sizeofTuripDataType(port->getType());
      if(rxBuf.writePoint > sizeofData){
        // データが十分に揃っている場合
        //　ユーザー領域へデータをコピー
        port->writeMemory(&rxBuf.buffer[1]);
        noInterrupts();
        //バッファのリセット
        rxBuf.writePoint = 0;
        rxBuf.readPoint = 0;
        txBuf.writePoint = 0;
        txBuf.readPoint = 0;
        // Set "Data type" bits.
        uint8_t typeFlags = 0;
        switch(port->getType()){
          case BOOL:
            typeFlags = 0x1;
            break;
          case INT8:
            typeFlags = 0x2;
            break;
          case INT16:
            typeFlags = 0x3;
            break;
          case INT32:
            typeFlags = 0x4;
            break;
          case INT64:
            typeFlags = 0x5;
            break;
          case FLOAT:
            typeFlags = 0xc;
            break;
          case DOUBLE:
            typeFlags = 0xd;
            break;
        }
        // ACK送信
        txBuf.buffer[txBuf.writePoint++] = 0x00 | typeFlags;
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
      // Set "Data type" bits.
      uint8_t typeFlags = 0;
      switch(port->getType()){
        case BOOL:
          typeFlags = 0x1;
          break;
        case INT8:
          typeFlags = 0x2;
          break;
        case INT16:
          typeFlags = 0x3;
          break;
        case INT32:
          typeFlags = 0x4;
          break;
        case INT64:
          typeFlags = 0x5;
          break;
        case FLOAT:
          typeFlags = 0xc;
          break;
        case DOUBLE:
          typeFlags = 0xd;
          break;
      }
      // 転送
      txBuf.buffer[txBuf.writePoint++] = 0x00 | typeFlags;
      int sizeofData = sizeofTuripDataType(port->getType());
      memcpy(&txBuf.buffer[txBuf.writePoint], port->readMemory(), sizeofData);
      txBuf.writePoint += sizeofData;
      // Serial.println(port->read<long>(), HEX);
      // for(int i=sizeofData; i>0; i--){
      //   Serial.print(txBuf.buffer[txBuf.writePoint - i], HEX);
      //   Serial.print(" ");
      // }
      // Serial.println();

      interrupts();
    }
  }
}
