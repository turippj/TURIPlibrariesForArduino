#include "TURIPshell.h"

void TURIPshell(const char* line, char* output, size_t maxLength){
  turipShellCommand cmd;
  turipShellResponse response;
  turipShellCommandParser(line, &cmd);
  response.id = 0;
  if(cmd.pathLength == 1 && cmd.method == TURIP_METHOD_GET){
    turipShellLocalGet(&cmd, &response);
    // Serial.print("Get[Local]: ");
  }else if(cmd.pathLength == 1 && cmd.method == TURIP_METHOD_POST){
    turipShellLocalPost(&cmd, &response);
    // Serial.print("Post[Local]: ");
  }else if(cmd.pathLength == 2 && cmd.method == TURIP_METHOD_GET){
    turipShellBridgeGet(&cmd, &response);
    // Serial.print("Get[Bridge]: ");
  }else if(cmd.pathLength == 2 && cmd.method == TURIP_METHOD_POST){
    turipShellBridgePost(&cmd, &response);
    // Serial.print("Post[Bridge]: ");
  }else{
    response.statusCode = 400;
  }
  char id[20];
  output[0] = '\0';
  strcat(output, "{\"status\":");
  if(cmd.pathLength == 0 && cmd.method == TURIP_METHOD_GET){
    strcat(output, "200,\"protocol\":\"TURIP\",\"id\":\"");
    turipIdIntToStr(TURIPserver.myId, id);
    strcat(output, id);
    strcat(output, "\"");
    TURIPclient.scan();
    if(TURIPclient.numofDevices > 0){
      strcat(output, ",\"bridge\":[");
      for(int i = 0;;){
        strcat(output, "\"");
        turipIdIntToStr(TURIPclient.devices[i].id, id);
        strcat(output, id);
        strcat(output, "\"");
        i++;
        if(i == TURIPclient.numofDevices) break;
        strcat(output, ",");
      }
      strcat(output, "]");
    }
  }else{
    char buf[8];
    sprintf(buf, "%d", response.statusCode);
    strcat(output, buf);
    if(response.id != 0){
      strcat(output, ",\"id\":\"");
      turipIdIntToStr(response.id, id);
      strcat(output, id);
      strcat(output, "\"");
    }
    if(response.statusCode == 200){
      strcat(output, ",\"port\":");
      sprintf(buf, "%d", response.port);
      strcat(output, buf);
      strcat(output, ",\"data\":");
      strcat(output, response.data);
    }
  }
  strcat(output, "}");
}

void turipShellLocalGet(turipShellCommand* cmd, turipShellResponse* response){
  response->id = TURIPserver.myId;
  response->port = (uint8_t)cmd->path[0];
  response->statusCode = 200;
  TURIPport* p = TURIPserver.getPort(response->port);
  if(p == NULL){
    response->statusCode = 404;
    return;
  }
  uint8_t portData[64];
  char s_portData[64];
  p->transmit(portData, 64);

  switch(p->type){
    case TURIP_TYPE_UNKNOWN:
      response->statusCode = 500;
      break;
    case TURIP_TYPE_BOOL:
      if(portData[0] == 0){
        sprintf(response->data, "false");
      } else if(portData[0] == 1) {
        sprintf(response->data, "true");
      } else {
        response->statusCode = 500;
      }
      break;
    case TURIP_TYPE_UINT8:
      sprintf(response->data, "%d", *((uint8_t*)portData));
      break;
    case TURIP_TYPE_INT8:
      sprintf(response->data, "%d", *((int8_t*)portData));
      break;
    case TURIP_TYPE_UINT16:
      sprintf(response->data, "%d", *((uint16_t*)portData));
      break;
    case TURIP_TYPE_INT16:
      sprintf(response->data, "%d", *((int16_t*)portData));
      break;
    case TURIP_TYPE_UINT32:
      sprintf(response->data, "%d", *((uint32_t*)portData));
      break;
    case TURIP_TYPE_INT32:
      sprintf(response->data, "%d", *((int32_t*)portData));
      break;
    // case TURIP_TYPE_UINT64:
    //   response->data = *((uint64_t*)portData);
    //   break;
    // case TURIP_TYPE_INT64:
    //   response->data = *((int64_t*)portData);
    //   break;
    case TURIP_TYPE_FLOAT:
      sprintf(response->data, "%f", *((float*)portData));
      break;
    // case TURIP_TYPE_DOUBLE:
    //   response->data = *((double*)portData);
    //   break;
    case TURIP_TYPE_STRING:
      sprintf(response->data, "\"%s\"", (char*)portData);
      break;
    default:
      response->statusCode = 500;
  }
}

void turipShellLocalPost(turipShellCommand* cmd, turipShellResponse* response){
  response->id = TURIPserver.myId;
  response->statusCode = 200;
  TURIPport* p = TURIPserver.getPort((uint8_t)cmd->path[0]);
  if(p == NULL){
    response->statusCode = 404;
    return;
  }
  response->port = (uint8_t)cmd->path[0];
  union {
    uint8_t ui8;
    uint16_t ui16;
    uint32_t ui32;
    uint64_t ui64;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f;
    double d;
  } buf;

  switch(p->type){
    case TURIP_TYPE_UNKNOWN:
      response->statusCode = 500;
      break;
    case TURIP_TYPE_BOOL:
      if(!strcmp(cmd->data, "false")){
        buf.i8 = 0;
        sprintf(response->data, "false");
      } else if(!strcmp(cmd->data, "true")){
        buf.i8 = 1;
        sprintf(response->data, "true");
      } else {
        response->statusCode = 400;
        break;
      }
      p->receive(&buf.ui8);
      break;
    case TURIP_TYPE_UINT8:
      buf.ui8 = (uint8_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.ui8);
      break;
    case TURIP_TYPE_INT8:
      buf.i8 = (int8_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.i8);
      break;
    case TURIP_TYPE_UINT16:
      buf.ui16 = (uint16_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.ui16);
      break;
    case TURIP_TYPE_INT16:
      buf.i16 = (int16_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.i16);
      break;
    case TURIP_TYPE_UINT32:
      buf.ui32 = (uint32_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.ui32);
      break;
    case TURIP_TYPE_INT32:
      buf.i32 = (int32_t)atoi(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%d", buf.i32);
      break;
    // case TURIP_TYPE_UINT64:
    //   buf.ui64 = (uint64_t)atoi(cmd->data);
    //   p->receive(&buf.ui8);
    //   response->data = buf.ui64;
    //   break;
    // case TURIP_TYPE_INT64:
    //   buf.i64 = (int64_t)atoi(cmd->data);
    //   p->receive(&buf.ui8);
    //   response->data = buf.i64;
    //   break;
    case TURIP_TYPE_FLOAT:
      buf.f = (float)atof(cmd->data);
      p->receive(&buf.ui8);
      sprintf(response->data, "%f", buf.f);
      break;
    // case TURIP_TYPE_DOUBLE:
    //   buf.d = (double)atof(cmd->data);
    //   p->receive(&buf.ui8);
    //   response->data = buf.d;
    //   break;
    case TURIP_TYPE_STRING:
      p->receive((uint8_t*)cmd->data);
      sprintf(response->data,  "\"%s\"", cmd->data);
      break;
    default:
      response->statusCode = 500;
  }
}

void turipShellBridgeGet(turipShellCommand* cmd, turipShellResponse* response){
  TURIPdevice dev;
  response->id = 0;

  if(dev.attach(cmd->path[0]) == 0){
    response->statusCode = 200;
    response->id = cmd->path[0];
    response->port = (uint8_t)cmd->path[1];

    switch(dev.getType(response->port)){
      case TURIP_TYPE_UINT8:
        sprintf(response->data,  "%d", dev.readUint8(response->port));
        break;
      case TURIP_TYPE_INT8:
        sprintf(response->data,  "%d", dev.readInt8(response->port));
        break;
      case TURIP_TYPE_UINT16:
        sprintf(response->data,  "%d", dev.readUint16(response->port));
        break;
      case TURIP_TYPE_INT16:
        sprintf(response->data,  "%d", dev.readInt16(response->port));
        break;
      case TURIP_TYPE_UINT32:
        sprintf(response->data,  "%d", dev.readUint32(response->port));
        break;
      case TURIP_TYPE_INT32:
        sprintf(response->data,  "%d", dev.readInt32(response->port));
        break;
      // case TURIP_TYPE_UINT64:
      //   response->data = dev.readUint64(cmd->port);
      //   break;
      // case TURIP_TYPE_INT64:
      //   response->data = dev.readInt64(cmd->port);
      //   break;
      case TURIP_TYPE_FLOAT:
        sprintf(response->data,  "%f", dev.readFloat(response->port));
        break;
      // case TURIP_TYPE_DOUBLE:
      //   response->data = dev.readDouble(cmd->port);
      //   break;
      case TURIP_TYPE_STRING:
        char sbuf[64];
        dev.readString(response->port, sbuf, 64);
        sprintf(response->data,  "\"%s\"", sbuf);
        break;
      default:
        response->statusCode = 500;
    }
  } else {
    response->statusCode = 404;
  }

}

void turipShellBridgePost(turipShellCommand* cmd, turipShellResponse* response){
  TURIPdevice dev;

  if(dev.attach(cmd->path[0]) == 0){
    response->statusCode = 200;
    response->id = cmd->path[0];
    response->port = (uint8_t)cmd->path[1];
    sprintf(response->data,  "%s", cmd->data);
    union {
      uint8_t ui8;
      uint16_t ui16;
      uint32_t ui32;
      uint64_t ui64;
      int8_t i8;
      int16_t i16;
      int32_t i32;
      int64_t i64;
      float f;
      double d;
    } buf;

    switch(dev.getType(response->port)){
      case TURIP_TYPE_UINT8:
        buf.ui8 = (uint8_t)atoi(cmd->data);
        dev.writeUint8(response->port, buf.ui8);
        sprintf(response->data,  "%d", buf.ui8);
        break;
      case TURIP_TYPE_INT8:
        buf.i8 = (int8_t)atoi(cmd->data);
        dev.writeInt8(response->port, buf.i8);
        sprintf(response->data,  "%d", buf.i8);
        break;
      case TURIP_TYPE_UINT16:
        buf.ui16 = (uint16_t)atoi(cmd->data);
        dev.writeUint16(response->port, buf.ui16);
        sprintf(response->data,  "%d", buf.ui16);
        break;
      case TURIP_TYPE_INT16:
        buf.i16 = (int16_t)atoi(cmd->data);
        dev.writeInt16(response->port, buf.i16);
        sprintf(response->data,  "%d", buf.i16);
        break;
      case TURIP_TYPE_UINT32:
        buf.ui32 = (uint32_t)atoi(cmd->data);
        dev.writeUint32(response->port, buf.ui32);
        sprintf(response->data,  "%d", buf.ui32);
        break;
      case TURIP_TYPE_INT32:
        buf.i32 = (int32_t)atoi(cmd->data);
        dev.writeInt32(response->port, buf.i32);
        sprintf(response->data,  "%d", buf.i32);
        break;
      // case TURIP_TYPE_UINT64:
      //   buf.ui64 = (uint64_t)atoi(cmd->data);
      //   dev.writeUint64(cmd->port, buf.ui64);
      //   response->data = buf.ui64;
      //   break;
      // case TURIP_TYPE_INT64:
      //   buf.i64 = (int64_t)atoi(cmd->data);
      //   dev.writeInt64(cmd->port, buf.i64);
      //   response->data = buf.i64;
      //   break;
      case TURIP_TYPE_FLOAT:
        buf.f = (float)atof(cmd->data);
        dev.writeFloat(response->port, buf.f);
        sprintf(response->data,  "%f", buf.f);
        break;
      // case TURIP_TYPE_DOUBLE:
      //   buf.d = (double)atof(cmd->data);
      //   dev.writeDouble(cmd->port, buf.d);
      //   response->data = buf.d;
      //   break;
      case TURIP_TYPE_STRING:
        dev.writeString(response->port, cmd->data);
        sprintf(response->data,  "\"%s\"", cmd->data);
        break;
      default:
        response->statusCode = 500;
    }
  } else {
    response->statusCode = 404;
  }
}

uint64_t turipIdStrToInt(const char* id){
  uint64_t _id = 0;
  char fullid[16 + 1];
  for (size_t i = 0; i < 16; i++) {
    fullid[i] = '0';
  }
  fullid[16] = '\0';
  int bufStlLength = strlen(id);
  int separatorIndex = -1;
  for (size_t i = 0; i < bufStlLength; i++) {
    if(id[i] == '-'){
      separatorIndex = i;
      break;
    }
  }

  int index = 15;
  if(separatorIndex != -1){
    for (int i = bufStlLength - 1; i > separatorIndex; i--) {
      fullid[index--] = id[i];
      if(index == 7) break;
    }
    index = 7;
    for (int i = separatorIndex - 1; i >= 0; i--) {
      fullid[index--] = id[i];
      if(index < 0) break;
    }
  }else{
    for (int i = bufStlLength - 1; i >= 0; i--) {
      fullid[index--] = id[i];
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

void turipIdIntToStr(uint64_t id, char* output){
  uint32_t model = id >> 32;
  uint32_t serial = (uint32_t)id;
  sprintf(output, "%x-%x", model, serial);
}

void turipShellCommandParser(const char* line, turipShellCommand* parsed){
  size_t lineLength = strlen(line);
  size_t lineReadPoint = 0;
  char buf[32];

  parsed->pathLength = 0;
  for (; lineReadPoint < lineLength; lineReadPoint++) {
    if(line[lineReadPoint] == ' '){
      break;
    }
    if(line[lineReadPoint] == '/'){
      continue;
    }
    if(line[lineReadPoint] >= '!' && line[lineReadPoint] <= '~'){
      size_t i = 0;
      for (lineReadPoint;
        line[lineReadPoint] >= '!' &&
        line[lineReadPoint] <= '~' &&
        line[lineReadPoint] != '/';
        lineReadPoint++) {
        buf[i++] = line[lineReadPoint];
      }
      buf[i] = '\0';
      if(i <= 2){
        parsed->path[parsed->pathLength++] = atoi(buf);
      }else{
        parsed->path[parsed->pathLength++] = turipIdStrToInt(buf);
      }
    }
    if(line[lineReadPoint] == ' ' || line[lineReadPoint] == '\0'){
      break;
    }
  }

  while(line[lineReadPoint] == ' '){
    lineReadPoint++;
  }

  if(line[lineReadPoint] >= '!' && line[lineReadPoint] <= '~'){
    parsed->method = TURIP_METHOD_POST;
    size_t i = 0;
    for (; line[lineReadPoint] >= '!' && line[lineReadPoint] <= '~'; lineReadPoint++) {
      parsed->data[i++] = line[lineReadPoint];
    }
    parsed->data[i] = '\0';
  }else{
    parsed->method = TURIP_METHOD_GET;
  }
}
