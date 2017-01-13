#ifndef TURIP_DATATYPE_H
#define TURIP_DATATYPE_H

/*
  TURIPdataType

  TURIPで用いるデータ型の列挙体

  BOOL: ブール型
  INT8~INT64: 8~64bit整数型
  FLOAT: 単精度(32bit)浮動小数点型
  DOUBLE: 倍精度(64bit)浮動小数点型
*/

typedef enum en_TURIPdataType{
  BOOL,INT8, INT16, INT32, INT64,
  FLOAT, DOUBLE
} TURIPdataType;

/*
int sizeofTuripDataType(TURIPdataType type)

TuripDataTypeのバイト数を返す。

TURIPdataType type: 対象のTURIPデータ型

return: uripDataTypeのバイト数
*/
int sizeofTuripDataType(TURIPdataType type);

#endif
