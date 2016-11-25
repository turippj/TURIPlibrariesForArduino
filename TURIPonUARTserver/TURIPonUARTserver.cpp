#include "TURIPonUARTserver.h"

void cl_TURIPonUARTserver::begin(){
  Serial.begin(115200);
}

void cl_TURIPonUARTserver::communicate(){
  StaticJsonBuffer<400> jsonBuffer;
  char*   CLIENT_JSON;
  void*   vdata;
  float*  fdata;
  int*    ldata;
  uint8_t tstatus = 500;
  //serverが返すjsonを作っておく
  JsonObject& server = jsonBuffer.createObject();
  //clientからのjsonを受信
  while(CLIENT_JSON = NULL){ CLIENT_JSON = Serial.read(); }

  //client_jsonのparse
  JsonObject& client = jsonBuffer.parseObject(CLIENT_JSON);
  char* chrport  = client["port"];
  char* method = client["method"];
  char* type   = client["type"]; //get時に得られるdataの型
  char* data   = client["data"];
  int port = atoi(chrport);

  if (strchr(data, '.') != NULL)
    *fdata = atof(data);
  else
    *ldata = atoi(data);

  if(method == "post"){
    if(*fdata != NULL){
      if(TURIPserver.write(port, fdata) == -1){tstatus = 404;} //dataが実数の時
      server["data"] = sprintf(data,"%f",fdata);
    }else if(*ldata != NULL){
      if(TURIPserver.write(port, ldata) == -1){tstatus = 404;} //dataが整数の時
      server["data"] = sprintf(data,"%d",ldata);
    }else{
      if(TURIPserver.write(port,  data) == -1){tstatus = 404;} //dataが文字列の時
      server["data"] = data;
    }
  }else{ //method == get
      if(TURIPserver.read(port, vdata) == -1){tstatus = 404;}
      if(type == "string") server["data"] = (char*)vdata;
      else if(type == "int")server["data"] = (int*)vdata;
      else if(type == "float")server["data"] = (float*)vdata;
  }


  //clientへの返答データの生成
  server["status"] = tstatus;
  server["port"]   = chrport;

  //clientへ返答
  server.printTo(Serial);
}
