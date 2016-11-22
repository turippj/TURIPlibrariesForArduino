#ifndef TURIP_ON_SPI_SERVER_H
#define TURIP_ON_SPI_SERVER_H
#include <Arduino.h>

struct st_TURIPonSPIserver_buffer{
  volatile uint8_t* buffer;
  volatile int readPoint, writePoint, maxLength;
};

class cl_TURIPonSPIserver{
public:
  void begin(uint32_t model, uint32_t serial);
  /*
  void begin(uint32_t model, uint32_t serial)
  初期化動作を行う（SPI通信の有効化、IDの登録）
  */

  int add(uint8_t port, void* data, size_t datasize);
  /*
  int add(uint8_t port, void* data, size_t datasize)
  ポートを追加する。
  現在登録されているポート数を返す。メモリ不足で追加できなかったらポート数の代わりに'-1'が返ってくる。
  */

  void update();
  /*
  void update();
  バッファに溜まった情報の一括処理を行う。
  ユーザーが任意のタイミングに合わせて行う。
  ぶっちゃけソフトウェア割り込みがあったらこんなの要らない・・・
  TODO: updateの自動実行を実現する
  */

  cl_TURIPonSPIserver();

private:
  size_t numof_ports;
  uint32_t cv_model, cv_serial;
  int serachPort(uint8_t port);
  struct st_portTable{
    uint8_t port;
    size_t datasize;
    void* data;
  } *portTable;
};

namespace ns_TURIPonSPIserver{
  extern volatile struct st_TURIPonSPIserver_buffer rxBuf,txBuf;
}
extern cl_TURIPonSPIserver TURIPonSPIserver;

#endif
