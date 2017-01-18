#include "TURIPserverSerial.h"

cl_TURIPserverSerial TURIPserverSerial;

void cl_TURIPserverSerial::begin(){
  Serial.begin(115200);
  Serial.println("{\"msg\":\"TURIPserverSerial started.\"}");
  bufferLen = 0;
}

void cl_TURIPserverSerial::update(){
  int c = Serial.read();
  if(c != -1){
    buffer[bufferLen++] = c;
    if(bufferLen == 64){
      bufferLen = 0;
      Serial.println("{\"msg\":\"EROOR: Too long command.\"}");
    }
  }else{
    return;
  }
  if(c == '\n'){
    buffer[bufferLen] = '\0';
    bufferLen = 0;
    // Serial.print("Command: ");
    // Serial.println(buffer);

    turipJsonRequest parsed = parse(buffer);

    // Serial.print("Method: ");
    // switch(parsed.type){ // 何故かここでは誤った結果を出す。（他は正常なのに・・・）
    //   case POST:  Serial.println("POST"); break;
    //   case GET:   Serial.println("GET");  break;
    //   default:    Serial.println("NONE");
    // }
    // Serial.print("Port: ");
    // Serial.println(parsed.port);
    // Serial.print("Type: ");
    // switch(parsed.type){
    //   case INT8:    Serial.println("INT8");   break;
    //   case INT16:   Serial.println("INT16");  break;
    //   case INT32:   Serial.println("INT32");  break;
    //   case INT64:   Serial.println("INT64");  break;
    //   case FLOAT:   Serial.println("FLOAT");  break;
    //   case DOUBLE:  Serial.println("DOUBLE"); break;
    //   case BOOL:    Serial.println("BOOL");   break;
    // }

    sendResponse(&parsed);

    if(parsed.data != NULL){
      delete parsed.data;
    }
  }
}

turipJsonRequest cl_TURIPserverSerial::parse(char input[]){
  StaticJsonBuffer<200> jsonBuffer;
  turipJsonRequest result;
  result.data = NULL;

  JsonObject& client = jsonBuffer.parseObject(input);
  result.port = atoi(client["port"]);
  const char* method = client["method"];
  const char* type   = client["type"]; //get時に得られるdataの型

  if(!strcmp("get", method)){
    result.method = GET;
  }else if(!strcmp("post", method)){
    result.method = POST;
  }else{
    result.method = NONE;
  }
  if(!strcmp(type, "INT8")){
    result.type = INT8;
    result.data = new int8_t;
    int8_t data = atoi(client["data"]);
    memcpy(result.data, &data, sizeof(int8_t));
  }else if(!strcmp(type, "INT16")){
    result.type = INT16;
    result.data = new int16_t;
    int16_t data = atoi(client["data"]);
    memcpy(result.data, &data, sizeof(int16_t));
  }else if(!strcmp(type, "INT32")){
    result.type = INT32;
    result.data = new int32_t;
    int32_t data = atoi(client["data"]);
    memcpy(result.data, &data, sizeof(int32_t));
  }else if(!strcmp(type, "INT64")){
    result.type = INT64;
    result.data = new int64_t;
    int64_t data = atoi(client["data"]);
    memcpy(result.data, &data, sizeof(int64_t));
  }else if(!strcmp(type, "FLOAT")){
    result.type = FLOAT;
    result.data = new float;
    float data = atof(client["data"]);
    memcpy(result.data, &data, sizeof(float));
  }else if(!strcmp(type, "DOUBLE")){
    result.type = DOUBLE;
    result.data = new double;
    double data = atof(client["data"]);
    memcpy(result.data, &data, sizeof(double));
  }else if(!strcmp(type, "BOOL")){
    result.type = BOOL;
    result.data = new int8_t;
    int8_t data = atoi(client["data"]);
    memcpy(result.data, &data, sizeof(int8_t));
  }
  return result;
}

void cl_TURIPserverSerial::sendResponse(turipJsonRequest* request){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  TURIPport* port = TURIPserver.get(request->port);
  if(port != NULL){
    root["status"] = "200";
    if(request->method == GET){
      switch(port->getType()){
        case INT8:
          int8_t data8;
          memcpy(&data8, port->readMemory(), sizeofTuripDataType(INT8));
          root.set<int8_t>("data", data8);
          break;
        case INT16:
          int16_t data16;
          memcpy(&data16, port->readMemory(), sizeofTuripDataType(INT16));
          root.set<int16_t>("data", data16);
          break;
        case INT32:
          int32_t data32;
          memcpy(&data32, port->readMemory(), sizeofTuripDataType(INT32));
          root.set<int32_t>("data", data32);
          break;
        case FLOAT:
          float dataf;
          memcpy(&dataf, port->readMemory(), sizeofTuripDataType(FLOAT));
          root.set<float>("data", dataf);
          break;
        case DOUBLE:
          double datad;
          memcpy(&datad, port->readMemory(), sizeofTuripDataType(DOUBLE));
          root.set<double>("data", datad);
          break;
      }
    }else if(request->method == POST){
      port->writeMemory(request->data);
    }
  }else{
    root["status"] = "404";
  }
  root.printTo(Serial);
  Serial.println();
}
