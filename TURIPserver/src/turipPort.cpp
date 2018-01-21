#include "TURIPport.h"

TURIPport::TURIPport(uint8_t portNum){
  // ポート番号の設定
  this->portNumber = portNum;
  // パーミッションのデフォルトをREADWRITEとする
  this->permission = TURIP_PERMISSION_RW;

  // トリガ関数は無効化する
  this->preReceiveFunc = NULL;
  this->postReceiveFunc = NULL;
  this->preTransmitFunc = NULL;
  this->postTransmitFunc = NULL;

  cache = NULL;
  cacheSize = 0;
  type = TURIP_TYPE_UNKNOWN;
}

int8_t TURIPport::readInt8(){
  if(type == TURIP_TYPE_INT8) return (int8_t&)cache;
  return 0;
}

int16_t TURIPport::readInt16(){
  if(type == TURIP_TYPE_INT16) return (int16_t&)cache;
  return 0;
}

int32_t TURIPport::readInt32(){
  if(type == TURIP_TYPE_INT32) return (int32_t&)cache;
  return 0;
}

int64_t TURIPport::readInt64(){
  if(type == TURIP_TYPE_INT64) return (int64_t&)cache;
  return 0;
}

uint8_t TURIPport::readUint8(){
  if(type == TURIP_TYPE_UINT8) return (uint8_t&)cache;
  return 0;
}

uint16_t TURIPport::readUint16(){
  if(type == TURIP_TYPE_UINT16) return (uint16_t&)cache;
  return 0;
}

uint32_t TURIPport::readUint32(){
  if(type == TURIP_TYPE_UINT32) return (uint32_t&)cache;
  return 0;
}

uint64_t TURIPport::readUint64(){
  if(type == TURIP_TYPE_UINT64) return (uint64_t&)cache;
  return 0;
}

float TURIPport::readFloat(){
  if(type == TURIP_TYPE_FLOAT) return (float&)cache;
  return 0;
}

double TURIPport::readDouble(){
  if(type == TURIP_TYPE_DOUBLE) return (double&)cache;
  return 0;
}

String TURIPport::readString(){
  String str;
  if(type == TURIP_TYPE_STRING){
    str += (char*)cache;
  }
  return str;
}

void TURIPport::writeInt8(int8_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_INT8){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(int8_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_INT8;
  }
  memcpy(cache, &data, sizeof(int8_t));
}

void TURIPport::writeInt16(int16_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_INT16){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(int16_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_INT16;
  }
  memcpy(cache, &data, sizeof(int16_t));
}

void TURIPport::writeInt32(int32_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_INT32){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(int32_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_INT32;
  }
  memcpy(cache, &data, sizeof(int32_t));
}

void TURIPport::writeInt64(int64_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_INT64){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(int64_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_INT64;
  }
  memcpy(cache, &data, sizeof(int64_t));
}

void TURIPport::writeUint8(uint8_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_UINT8){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(uint8_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_UINT8;
  }
  memcpy(cache, &data, sizeof(uint8_t));
}

void TURIPport::writeUint16(uint16_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_UINT16){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(uint16_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_UINT16;
  }
  memcpy(cache, &data, sizeof(uint16_t));
}

void TURIPport::writeUint32(uint32_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_UINT32){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(uint32_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_UINT32;
  }
  memcpy(cache, &data, sizeof(uint32_t));
}

void TURIPport::writeUint64(uint64_t data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_UINT64){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(uint64_t);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_UINT64;
  }
  memcpy(cache, &data, sizeof(uint64_t));
}

void TURIPport::writeFloat(float data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_FLOAT){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(float);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_FLOAT;
  }
  memcpy(cache, &data, sizeof(float));
}

void TURIPport::writeDouble(double data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(type != TURIP_TYPE_DOUBLE){
    if(cache != NULL){
      delete[] cache;
    }
    cacheSize = sizeof(double);
    cache = new uint8_t[cacheSize];
    type = TURIP_TYPE_DOUBLE;
  }
  memcpy(cache, &data, sizeof(double));
}

void TURIPport::writeString(String data){
  if(permission == TURIP_PERMISSION_RO){
    return;
  }
  if(cache != NULL){
    delete[] cache;
  }
  cacheSize = data.length() + 1;
  cache = new uint8_t[cacheSize];
  type = TURIP_TYPE_STRING;
  data.toCharArray((char*)cache, cacheSize);
}

int TURIPport::receive(uint8_t* data){
  if(preReceiveFunc != NULL) preReceiveFunc();
  if(type == TURIP_TYPE_STRING){
    int dataSize = strlen(data) + 1;
    if(dataSize > cacheSize){
      if(cache != NULL){
        delete[] cache;
      }
      cacheSize = dataSize;
      cache = new uint8_t[cacheSize];
    }
    strcpy(cache, (char*)data);
  }else{
    memcpy(cache, data, cacheSize);
  }
  if(postReceiveFunc != NULL) postReceiveFunc();
}

int TURIPport::transmit(uint8_t* data, int maxLength){
  if(preTransmitFunc != NULL) preTransmitFunc();
  if(cacheSize > maxLength){
    memcpy(data, cache, maxLength);
  }else{
    memcpy(data, cache, cacheSize);
  }
  if(postTransmitFunc != NULL) postTransmitFunc();
}
