#ifndef TURIP_SERVER_H
#define TURIP_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/defines.h"
#include "lib/turipPort.h"

#define MAX_NUMOF_PORTS 32

/*
class cl_TURIPserver

ポートの集中管理を行う。
cl_TURIPserver TURIPserverのみ存在する。
*/
class cl_TURIPserver{
public:

  cl_TURIPserver();

  void begin(uint32_t model, uint32_t serial);
  /*
  サーバ機能を有効化させる。

  uint32_t model: TURIP型番
  uint32_t serial: TURIPシリアル番号
  */

  TURIPport* newPort(uint8_t portNumber);
  /*
  ポートを追加する。

  TURIPport* newPort: 追加するTURIPportオブジェクトのポインタ

  return: 0:成功 -1:失敗
  */

  TURIPport* getPort(uint8_t portNumber);
  /*
  ポートを検索する

  uint8_t port: 呼び出すポート番号

  return: 成功:該当するTURIPportオブジェクトのポインタ 失敗:NULLポインタ
  */

  uint64_t myId;

private:
  // 保有しているポートの数
  int numofPorts;

  // TURIPportオブジェクトポインタの保管
  TURIPport* portList[MAX_NUMOF_PORTS];

};
extern cl_TURIPserver TURIPserver;

#endif
