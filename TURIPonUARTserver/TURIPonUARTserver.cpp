#include "TURIPonUARTserver.h"

void begin(){
  Serial.begin(115200);
}


void get(uint64_t* id, uint8_t* port, char* method, char* data){
  while(CLIENT_JSON = NULL){ CLIENT_JSON = Serial.read(); }
  JsonObject& client_req = jsonBuffer.parseObject(CLIENT_JSON);
  char* chrid     = client_req["id"];
  char* chrport   = client_req["port"];
  method = client_req["method"];
  data   = client_req["method"];
  id   = strtol(id, &endptr, 16);
  port = strtol(port, &endptr, 16);
  if(getPort(port) == NULL) return -1;
  return 0;
}


void put(uint64_t id, uint8_t status, uint8_t port, char* data){
  JsonObject& server = jsonBuffer.createObject();
  server["id"] = id;
  server["status"] = tstatus;
  server["port"] = port;
  server["data"] = data;
  server.printTo(Serial);
}
