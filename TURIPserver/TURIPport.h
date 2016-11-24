#ifndef TURIP_PORT_H
#define TURIP_PORT_H

typedef enum en_TURIPdataType{
  BIN, STRING,
  INT8, UINT8,
  INT16, UINT16,
  INT32, UINT32,
  INT64, UINT64,
  FLOAT32, FLOAT64
} TURIPdataType;

typedef enum en_TURIPportPermission{
  DENY, READ, WRITE, READWRITE
} TURIPportPermission;

class cl_TURIPport{
public:
  /*
  コンストラクタ
  */
  cl_TURIPport(
    uint8_t port, void* data, TURIPdataType type,
    TURIPportPermission permission);

  cl_TURIPport(
    uint8_t port, void* data, TURIPdataType type,
    TURIPportPermission permission,
    int (*fn_preCallInRead)(void),
    int (*fn_preCallInWrite)(void),
    int (*fn_postCallInRead)(void),
    int (*fn_postCallInWrite)(void));

  /*
  int read(void* data)
  ポートからデータを読み取ります。
  void* data: ポートが管理する実データの先頭アドレス
  return: 成功->0, 失敗->-1
  */
  int read(void* data);

  /*
  int write(const void* data)
  ポートへデータを書き込みます。
  const void* data: ポートに書き込む（コピーする）データの先頭アドレス
  return: 成功->0, 失敗->-1
  */
  int write(const void* data);

private:
  // uint8_t port: このポートのポート番号
  uint8_t port;

  // void* data: データを保管するグローバル変数のアドレス
  void* data;

  // TURIPdataType type: このポートが管理するデータの型
  TURIPdataType type;

  // TURIPportPermission permission: このポートへのアクセス許可の定義
  TURIPportPermission permission;

  /*
  int (*fn_[pre/post]CallIn[Read/Write])()
  ポート読み書きの前後に実行する関数の定義
  int (*fn_preCallInRead)(): ポートからの読み込み前に実行
  int (*fn_preCallInWrite)(): ポートへの書き込み前に実行
  int (*fn_postCallInRead)(): ポートからの読み込み後に実行
  int (*fn_postCallInWrite)(): ポートへの書き込み後に実行
  実行する関数が無い場合: NULL(デフォルト)
  */
  int (*fn_preCallInRead)(void);
  int (*fn_preCallInWrite)(void);
  int (*fn_postCallInRead)(void);
  int (*fn_postCallInWrite)(void);

  /*
  int getDatasize(): ポート内データのデータサイズを取得する
  const void* data: サイズを調べるデータのポインタ
  return: 成功: データサイズ(bytes), 失敗: -1
  */
  int getDatasize(const void* data);
};

#endif
