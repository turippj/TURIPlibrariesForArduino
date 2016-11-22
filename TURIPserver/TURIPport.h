#ifndef TURIP_PORT_H
#define TURIP_PORT_H

typedef enum en_TURIPdataType{
  BINARY = 0, TEXT,
  INT8, UINT8,
  INT16, UINT16,
  INT32, UINT32,
  INT64, UINT64,
  FLOAT32, FLOAT64
} TURIPdataType;

typedef enum en_TURIPportPermission{READ, WRITE, READWRITE} TURIPportPermission;

class TURIPport{
public:
  TURIPport(uint8_t port, void* data, size_t dataSize);
  int read(void* data);
  /*
  ポートからデータを読み取る
  データ長をreturnする
  data: 転送先アドレス
  */
  int write(const void* data);
  /*
  ポートへデータを送る
  データ長をreturnする
  data: 転送元アドレス
  */
public:
  uint8_t port;
  void* data;
  size_t sizeof_data;
  TURIPportPermission permission;
  TURIPdataType type;
};


#endif
