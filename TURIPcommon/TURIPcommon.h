#ifndef TURIP_COMMON_H
#define TURIP_COMMON_H

/*
  TURIPdataType

  TURIPで用いるデータ型の列挙体

  BOOL: ブール型
  INT8~INT64: 8~64bit整数型
  FLOAT: 単精度(32bit)浮動小数点型
  DOUBLE: 倍精度(64bit)浮動小数点型

  備考:列挙体内のマッピング
  MSB\7\6\5\4\3\2\1\0\LSB
  0-3:データサイズ(bytes)
  4,5:データタイプ
  6,7:(予約)
*/

typedef enum en_TURIPdataType{
  BOOL  = 0x01,
  INT8  = 0x11,
  INT16 = 0x12,
  INT32 = 0x14,
  INT64 = 0x18,
  FLOAT = 0x44,
  DOUBLE= 0x48
} TURIPdataType;

/*
int sizeofTuripDataType(TURIPdataType type)

TuripDataTypeのバイト数を返す。

TURIPdataType type: 対象のTURIPデータ型

return: uripDataTypeのバイト数
*/
int sizeofTuripDataType(TURIPdataType type);

#endif
