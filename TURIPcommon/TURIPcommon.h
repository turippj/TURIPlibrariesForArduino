#ifndef TURIP_COMMON_H
#define TURIP_COMMON_H

/*
  TURIPdataType

  TURIPで用いるデータ型の列挙体

  BOOL: ブール型
  INT8~INT64: 8~64bit整数型
  FLOAT: 単精度(32bit)浮動小数点型
  DOUBLE: 倍精度(64bit)浮動小数点型
*/

typedef enum en_TURIPdataType{
  UNKNOWN = 0x00,
  BOOL    = 0x01,
  UINT8   = 0x02,
  INT8    = 0x03,
  UINT16  = 0x04,
  INT16   = 0x05,
  UINT32  = 0x06,
  INT32   = 0x07,
  UINT64  = 0x08,
  INT64   = 0x09,
  FLOAT   = 0x0a,
  DOUBLE  = 0x0b,
  STRING  = 0x0c,
  BINARY  = 0x0d
} TURIPdataType;

/*
int sizeofTuripDataType(TURIPdataType type)

TuripDataTypeのバイト数を返す。

TURIPdataType type: 対象のTURIPデータ型

return: uripDataTypeのバイト数
*/
int sizeofTuripDataType(TURIPdataType type);

#endif
