#ifndef TURIP_ON_SPI_SERVER_H
#define TURIP_ON_SPI_SERVER_H
#include <Arduino.h>
#include "TURIPserver.h"

struct st_TURIPserverSPI_buffer{
  uint8_t* buffer;
  int readPoint, writePoint, maxLength;
};

class cl_TURIPserverSPI{
public:
  cl_TURIPserverSPI();

  void begin();
  /*
  初期化動作を行う（SPI通信の有効化）
  */

  void update();
  /*
  void update();
  バッファに溜まった情報の一括処理を行う。
  ユーザーが任意のタイミングに合わせて行う。
  ぶっちゃけソフトウェア割り込みがあったらこんなの要らない・・・
  TODO: updateの自動実行を実現する
  */
};

namespace ns_TURIPserverSPI{
  extern struct st_TURIPserverSPI_buffer rxBuf,txBuf;
}

extern cl_TURIPserverSPI TURIPserverSPI;

#endif
