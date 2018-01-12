#ifndef TURIP_SERVER_H
#define TURIP_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "turipDefines.h"
#include "turipPort.h"

/*
class cl_TURIPserver

ポートの集中管理を行う。
cl_TURIPserver TURIPserverのみ存在する。
*/
class cl_TURIPserver{
public:
  /*
  コンストラクタ
  */
  cl_TURIPserver();

  /*
  void begin(uint32_t model, uint32_t serial)

  サーバ機能を有効化させる。

  uint32_t model: TURIP型番
  uint32_t serial: TURIPシリアル番号
  */
  void begin(uint32_t model, uint32_t serial);

  /*
  int add(TURIPport* newPort)

  ポートを追加する。

  TURIPport* newPort: 追加するTURIPportオブジェクトのポインタ

  return: 0:成功 -1:失敗
  */
  int add(TURIPport* newPort);

  /*
  TURIPport* get(uint8_t port)

  ポートを検索する

  uint8_t port: 呼び出すポート番号

  return: 成功:該当するTURIPportオブジェクトのポインタ 失敗:NULLポインタ
  */
  TURIPport* get(uint8_t port);

private:
  // 保有しているポートの数
  int numofPorts;

  // TURIPportオブジェクトポインタの保管
  TURIPport* portList[32];

  // TURIP型番を取り扱うTURIPportオブジェクト
  TURIPport* portOfModel;

  // TURIPシリアル番号を取り扱うTURIPportオブジェクト
  TURIPport* portOfSerial;
};
extern cl_TURIPserver TURIPserver;

#endif
