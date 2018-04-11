#include "TURIPserverSPI.h"

using namespace ns_TURIPserverSPI;

struct st_TURIPserverSPI_buffer ns_TURIPserverSPI::rxBuf;
struct st_TURIPserverSPI_buffer ns_TURIPserverSPI::txBuf;
cl_TURIPserverSPI TURIPserverSPI;

cl_TURIPserverSPI::cl_TURIPserverSPI(){
  rxBuf.maxLength = 32; rxBuf.readPoint = 0; rxBuf.writePoint = 0;
  txBuf.maxLength = 32; txBuf.readPoint = 0; txBuf.writePoint = 0;
  rxBuf.buffer = new uint8_t[rxBuf.maxLength];
  txBuf.buffer = new uint8_t[txBuf.maxLength];
}

void cl_TURIPserverSPI::begin(){
#if defined(AVR_UNO) || defined(AVR_PRO)
  // Setting SPI
  DDRB = DDRB | 0B00010000;  // Set "OUTPUT" MISO
  PORTB = PORTB | 0B00011000;  // Pullup MISO,MOSI
  SPDR = 0xff;
  SPCR = _BV(SPE) | _BV(SPIE);

  // Setting SS interrupt
  // PCMSK0 = _BV(PCINT1);
  // PCICR = PCICR | _BV(PCIE0);

  sei();
#endif
}

void cl_TURIPserverSPI::update(){
#if defined(AVR_UNO) || defined(AVR_PRO)
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
    TURIPport* port = TURIPserver.getPort(rxBuf.buffer[0] & 0x7f);
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
      int sizeofData = port->cacheSize;
      if(rxBuf.writePoint > sizeofData){
        // データが十分に揃っている場合
        //　ユーザー領域へデータをコピー
        port->receive(&rxBuf.buffer[1]);
        noInterrupts();
        //バッファのリセット
        rxBuf.writePoint = 0;
        rxBuf.readPoint = 0;
        txBuf.writePoint = 0;
        txBuf.readPoint = 0;
        // Set "Data type" bits.
        uint8_t typeFlags = (uint8_t)port->type;
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
      uint8_t typeFlags = (uint8_t)port->type;
      // 転送
      txBuf.buffer[txBuf.writePoint++] = 0x00 | typeFlags;
      int sizeofData = port->cacheSize;
      port->transmit(&txBuf.buffer[txBuf.writePoint], txBuf.maxLength - 1);
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
#endif
}
