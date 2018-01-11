#ifndef TURIP_SERVER_H
#define TURIP_SERVER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <TURIPcommon.h>
#include "TURIPportPermission.h"

/*
class TURIPport

TURIPのポートの管理を行う。
1つのインスタンス毎に1つのポートを管理する。
*/
class TURIPport{
public:
  /*
  コンストラクタ

  ポートの初期設定を行う。

  int portNum: 割り当てるポート番号
  TURIPdataType type: TURIP仕様のデータ型
  */
  TURIPport(int portNum, TURIPdataType type);

  /*
  setReadProcess(int (*function)(void* data))

  clientからReadされたときに実行する関数を登録する。
  最新のデータを確実に送るために用いる。
  登録した関数は、clientにデータを送る前に実行される。

  int (*function)(): 実行する関数(NULLで無効化)
    return: 成功:0, 失敗:-1
  */
  void setReadProcess(int (*function)());

  /*
  setWriteProcess(int (*function)())

  clientからWriteされたされたときに実行する関数を登録する。
  writeされた直後かつ、ステータスを返す前に実行したい関数のトリガ。
  登録した関数は、writeMemory()中で実行される。

  int (*function)(): 実行する関数(NULLで無効化)
    return: 成功:0, 失敗:-1
  */
  void setWriteProcess(int (*function)());

  /*
  template <typename T> T read()

  ポートからデータを読み取る。

  return: データ
  */
  template <typename T> T read(){
    return *(T*)(this->data);
  }

  char* read(){
    return this->data;
  }

  /*
  template <typename T> void write(T data)

  ポートへデータを書き込む。

  data: 書き込むデータ
  */
  template <typename T> void write(T data){
    memcpy(this->data, &data, sizeofTuripDataType(this->type));
  }

  void write(const char* data){
    if(this->stringLength >= strlen(data)){
      strcpy(this->data, data);
    }
  }

  /*
  void* readMemory()

  物理層ライブラリがポートからデータを読み取るときに使う。
  また、setReadProcess()で登録された関数を読み取り前に実行する。

  return: 成功:データ格納領域のポインタ, 失敗:NULLポインタ
  */
  void* readMemory();

  /*
  int writeMemory(void* data)

  物理層ライブラリがポートへデータを書き出すときに使う。
  また、setWriteProcess()で登録された関数を書き出し後に実行する。

  void* data: バッファ領域の先頭アドレス

  return: 成功:0, 失敗:-1
  */
  int writeMemory(void* data);

  /*
  TURIPdataType getType()

  TURIPデータ型を返す

  return: このポートのTURIPデータ型
  */
  TURIPdataType getType();

  /*
  void setType(TURIPdataType newType)

  このクラスが取り扱うTURIPデータ型を変更する

  TURIPdataType newType: このポートの新しいTURIPデータ型
  */
  void setType(TURIPdataType newType);

  /*
  void setStrlen(unsigned int length)

  TURIPデータ型が"STRING"のときのサイズを指定する。

  unsigned int length: バッファのサイズ(\0は除く)
  */
  void setStrlen(unsigned int length);

  /*
  uint8_t port

  このポートのポート番号(範囲: 1-126)
  */
  int portNumber;

  /*
  TURIPportPermission permission

  このポートのパーミッション
  */
  TURIPportPermission permission;

private:
  /*
  TURIPdataType type

  このポートのTURIPデータ型
  データ型はデータ保管領域のサイズに関わるので途中変更できない
  */
  TURIPdataType type;

  /*
  unsigned int stringLength

  TURIPデータ型が"STRING"のときの\0を除いたバッファサイズ
  */
  unsigned int stringLength;

  /*
  void* data

  データ保管領域の先頭アドレス
  適宜サイズを変更して使用する。
  */
  void* volatile data;

  /*
  int (*fn_readProcess)(void)

  clientからReadされたときに実行する関数。
  関数は、clientにデータを送る前に実行される。
  実行する関数が無い場合はNULLが入る(デフォルト)。

  return: 成功:0, 失敗:-1
  */
  int (*fn_readProcess)(void);

  /*
  int (*fn_writeProcess)(void)

  clientからWriteされたときに実行する関数。
  関数は、clientからデータを受け取り、レスポンスを返す前に実行される。
  実行する関数が無い場合はNULLが入る(デフォルト)。

  return: 成功:0, 失敗:-1
  */
  int (*fn_writeProcess)(void);
};


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
