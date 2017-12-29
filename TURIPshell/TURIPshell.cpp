#include "TURIPshell.h"
#include <Arduino.h>

String TURIPshell(String line){
  char strBuf[64];
  line.toCharArray(strBuf, 64);
  return TURIPshell(strBuf);
}

String TURIPshell(const char* line){
  int numofSegment = 0;
  char* method = NULL;
  char* id = NULL;
  char* port = NULL;
  char* type = NULL;
  char* data = NULL;
  int numofDev = 0;
  uint64_t* devId = NULL;
  String response;
  char strBuf[64];
  char strBuf2[32];

  numofSegment = getSegment(line, 0, &method);

  if(!strcmp(method, "get")){  // get method
    if(isOptionExist(line, "-b") && isOptionExist(line, "-p")){
      getArgFromOption(line, "-b", &id);
      getArgFromOption(line, "-p", &port);
      strReadAcrossBridge(id, port).toCharArray(strBuf2, 32);
      sprintf(strBuf, "{\"status\":200,\"id\":\"%s\",\"port\":%s,", id, port);
      response += strBuf;
      sprintf(strBuf, "\"data\":%s}", strBuf2);
      response += strBuf;
    }else if(isOptionExist(line, "-p")){
      if(port != NULL) delete[] port;
      getArgFromOption(line, "-p", &port);
      strReadFromPort(port).toCharArray(strBuf2, 32);
      sprintf(strBuf, "{\"status\":200,\"port\":%s,", port);
      response += strBuf;
      sprintf(strBuf, "\"data\":%s}", strBuf2);
      response += strBuf;
    }else{
      response += "{\"status\":400}";
    }
  }else if(!strcmp(method, "post")){  // post method
    if(isOptionExist(line, "-b") && isOptionExist(line, "-p")){
      getArgFromOption(line, "-b", &id);
      getArgFromOption(line, "-p", &port);
      getSegment(line, numofSegment - 1, &data);
      strWriteAcrossBridge(id, port, data).toCharArray(strBuf2, 32);
      sprintf(strBuf, "{\"status\":200,\"id\":\"%s\",\"port\":%s,", id, port);
      response += strBuf;
      sprintf(strBuf, "\"data\":%s}", strBuf2);
      response += strBuf;
    }else if(isOptionExist(line, "-p")){
      if(port != NULL) delete[] port;
      getArgFromOption(line, "-p", &port);
      if(data != NULL) delete[] data;
      getSegment(line, numofSegment - 1, &data);
      strWriteToPort(port, data).toCharArray(strBuf2, 32);
      sprintf(strBuf, "{\"status\":200,\"port\":%s,", port);
      response += strBuf;
      sprintf(strBuf, "\"data\":%s}", strBuf2);
      response += strBuf;
    }else{
      response += "{\"status\":400}";
    }
  }else if(!strcmp(method, "bridge")){  // bridge method
    if(numofSegment == 1){
      if(devId != NULL) delete[] devId;
      numofDev = TURIPclient.scan();
      response += "{\"status\":200,\"id\":[";
      for (size_t i = 0; i < numofDev; i++) {
        sprintf(strBuf, "\"%X-%X\"", (uint32_t)(TURIPclient.idList[i] >> 32), (uint32_t)(TURIPclient.idList[i]));
        response += strBuf;
        if(i != numofDev - 1){
          response += ",";
        }
      }
      response += "]}";
    }else{
      response += "{\"status\":400}";
    }
  }else{
    response += "{\"status\":400}";
  }

  if(method != NULL) delete[] method;
  if(id != NULL) delete[] id;
  if(port != NULL) delete[] port;
  if(type != NULL) delete[] type;
  if(data != NULL) delete[] data;
  if(devId != NULL) delete[] devId;

  return response;
}

String strReadAcrossBridge(const char* id, const char* port){
  String response;
  uint64_t _id = strToId(id);
  uint8_t _port = atoi(port);
  TURIP dev;

  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f;
    double d;
  } portBuf;
  char str[32];

  if(dev.attach(_id) == 0){
    TURIPdataType _type = dev.getType(_port);
    switch(_type){
      case INT8:
        dev.read(_port, _type, &portBuf.i8);
        sprintf(str, "%d", portBuf.i8);
        break;
      case INT16:
        dev.read(_port, _type, &portBuf.i16);
        sprintf(str, "%d", portBuf.i16);
        break;
      case INT32:
        dev.read(_port, _type, &portBuf.i32);
        sprintf(str, "%d", portBuf.i32);
        break;
      case INT64:
        dev.read(_port, _type, &portBuf.i64);
        sprintf(str, "%d", portBuf.i64);
        break;
      case FLOAT:
        dev.read(_port, _type, &portBuf.f);
        // sprintf(str, "%f", portBuf.f);
        dtostrf(portBuf.f, 8, 4, str);  // Alternative function in Arduino
        break;
      case DOUBLE:
        dev.read(_port, _type, &portBuf.d);
        dtostrf(portBuf.f, 8, 4, str);
        break;
      case BOOL:
        dev.read(_port, _type, &portBuf.i8);
        if(portBuf.i8){
          sprintf(str, "true");
        }else{
          sprintf(str, "false");
        }
        break;
    }
    response += str;
    response.trim();
  }
  return response;
}

String strWriteAcrossBridge(const char* id, const char* port, const char* data){
  String response;
  uint64_t _id = strToId(id);
  uint8_t _port = atoi(port);
  TURIP dev;
  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f;
    double d;
  } portBuf;
  char str[32];

  if(dev.attach(_id) == 0){
    TURIPdataType _type = dev.getType(_port);
    switch(_type){
      case INT8:
        portBuf.i8 = atoi(data);
        dev.write(_port, _type, &portBuf.i8);
        break;
      case INT16:
        portBuf.i16 = atoi(data);
        dev.write(_port, _type, &portBuf.i16);
        break;
      case INT32:
        portBuf.i32 = atoi(data);
        dev.write(_port, _type, &portBuf.i32);
        break;
      case INT64:
        portBuf.i64 = atoi(data);
        dev.write(_port, _type, &portBuf.i64);
        break;
      case FLOAT:
        portBuf.f = atof(data);
        dev.write(_port, _type, &portBuf.f);
        break;
      case DOUBLE:
        portBuf.d = atof(data);
        dev.write(_port, _type, &portBuf.d);
        break;
      case BOOL:
        if(!strcmp(data, "true")){
          portBuf.i8 = 1;
        }else{
          portBuf.i8 = 0;
        }
        dev.write(_port, _type, &portBuf.i8);
        break;
    }
  }

  return strReadAcrossBridge(id, port);
}

String strReadFromPort(const char* port){
  String response;
  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f;
    double d;
  } portBuf;
  char str[32];
  TURIPport* selectedPort = TURIPserver.get(atoi(port));

  if(selectedPort != NULL){
    switch(selectedPort->getType()){
      case INT8:
        memcpy(&portBuf.i8, selectedPort->readMemory(), sizeofTuripDataType(INT8));
        sprintf(str, "%d", portBuf.i8);
        break;
      case INT16:
        memcpy(&portBuf.i16, selectedPort->readMemory(), sizeofTuripDataType(INT16));
        sprintf(str, "%d", portBuf.i16);
        break;
      case INT32:
        memcpy(&portBuf.i32, selectedPort->readMemory(), sizeofTuripDataType(INT32));
        sprintf(str, "%d", portBuf.i32);
        break;
      case INT64:
        memcpy(&portBuf.i64, selectedPort->readMemory(), sizeofTuripDataType(INT64));
        sprintf(str, "%X", portBuf.i64);
        break;
      case FLOAT:
        memcpy(&portBuf.f, selectedPort->readMemory(), sizeofTuripDataType(FLOAT));
        // sprintf(str, "%f", portBuf.f);  // Not work in Arduino
        dtostrf(portBuf.f, 8, 4, str);  // Alternative function in Arduino
        break;
      case DOUBLE:
        memcpy(&portBuf.d, selectedPort->readMemory(), sizeofTuripDataType(DOUBLE));
        // sprintf(str, "%f", portBuf.d);
        dtostrf(portBuf.d, 8, 4, str);
        break;
      case BOOL:
        memcpy(&portBuf.i8, selectedPort->readMemory(), sizeofTuripDataType(BOOL));
        if(portBuf.i8){
          sprintf(str, "true");
        }else{
          sprintf(str, "false");
        }
        break;
    }
    response += str;
    response.trim();
  }
  return response;
}

String strWriteToPort(const char* port, char* data){
  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f;
    double d;
  } portBuf;

  TURIPport* selectedPort = TURIPserver.get(atoi(port));

  if(selectedPort != NULL){
    switch(selectedPort->getType()){
      case INT8:
        portBuf.i8 = atoi(data);
        selectedPort->writeMemory(&portBuf.i8);
        break;
      case INT16:
        portBuf.i16 = atoi(data);
        selectedPort->writeMemory(&portBuf.i16);
        break;
      case INT32:
        portBuf.i32 = atoi(data);
        selectedPort->writeMemory(&portBuf.i32);
        break;
      case INT64:
        portBuf.i64 = atoi(data);
        selectedPort->writeMemory(&portBuf.i64);
        break;
      case FLOAT:
        portBuf.f = atof(data);
        selectedPort->writeMemory(&portBuf.f);
        break;
      case DOUBLE:
        portBuf.d = atof(data);
        selectedPort->writeMemory(&portBuf.d);
        break;
      case STRING:
        selectedPort->writeMemory(data);
        break;
      case BOOL:
        if(!strcmp(data, "true")){
          portBuf.i8 = 1;
        }else{
          portBuf.i8 = 0;
        }
        selectedPort->writeMemory(&portBuf.i8);
        break;
    }
  }
  return strReadFromPort(port);
}

uint64_t strToId(const char* id){
  uint64_t _id = 0;
  char fullid[16 + 1];
  for (size_t i = 0; i < 16; i++) {
    fullid[i] = '0';
  }
  fullid[16] = '\0';
  String bufStr = id;
  int bufStlLength = bufStr.length();
  int separatorIndex = bufStr.indexOf('-');

  int index = 15;
  if(separatorIndex != -1){
    for (int i = bufStlLength - 1; i > separatorIndex; i--) {
      fullid[index--] = bufStr[i];
      if(index == 7) break;
    }
    index = 7;
    for (int i = separatorIndex - 1; i >= 0; i--) {
      fullid[index--] = bufStr[i];
      if(index < 0) break;
    }
  }else{
    for (int i = bufStlLength - 1; i >= 0; i--) {
      fullid[index--] = bufStr[i];
      if(index < 0) break;
    }
  }

  for (size_t i = 0; i < 16; i++) {
    uint8_t bin;
    switch (fullid[i]) {
      case '0': bin = 0x0; break;
      case '1': bin = 0x1; break;
      case '2': bin = 0x2; break;
      case '3': bin = 0x3; break;
      case '4': bin = 0x4; break;
      case '5': bin = 0x5; break;
      case '6': bin = 0x6; break;
      case '7': bin = 0x7; break;
      case '8': bin = 0x8; break;
      case '9': bin = 0x9; break;
      case 'a': bin = 0xa; break;
      case 'b': bin = 0xb; break;
      case 'c': bin = 0xc; break;
      case 'd': bin = 0xd; break;
      case 'e': bin = 0xe; break;
      case 'f': bin = 0xf; break;
    }
    _id |= (uint64_t)bin << (60 - 4 * i);
  }

  return _id;
}

TURIPdataType strToType(const char* type){
  if(!strcmp(type, "int8")) return INT8;
  if(!strcmp(type, "int16")) return INT16;
  if(!strcmp(type, "int32")) return INT32;
  if(!strcmp(type, "int64")) return INT64;
  if(!strcmp(type, "float")) return FLOAT;
  if(!strcmp(type, "double")) return DOUBLE;
  if(!strcmp(type, "bool")) return BOOL;
}
