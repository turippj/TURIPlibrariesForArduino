#include "TURIPdataType.h"

int sizeofTuripDataType(TURIPdataType type){
  switch(type){
    case INT8:
    case BOOL:
      return 1;

    case INT16:
      return 2;

    case INT32:
    case FLOAT:
      return 4;

    case INT64:
    case DOUBLE:
      return 8;
  }
  return 0;
}
