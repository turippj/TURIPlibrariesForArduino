#ifndef TURIP_SERVER_SERIAL_H
#define TURIP_SERVER_SERIAL_H

#include "TURIPserver.h"
#include <Arduino.h>
#include <ArduinoJson.h>

typedef enum en_turipRequestMethod{
  NONE,GET,POST
} turipRequestMethod;

typedef struct st_turipJsonRequest{
  turipRequestMethod method;
  int port;
  TURIPdataType type;
  void* data;
} turipJsonRequest;

class cl_TURIPserverSerial{
public:
  /*
  void begin()

  初期化動作を行う(UART通信の開始)
  */
  void begin();

  /*
  void update()

  clientの要求を受け付けてから返答を返すまでやるやつ
  */
  void update();

private:
  char buffer[64];
  int bufferLen;

  turipJsonRequest parse(char input[]);
  void sendResponse(turipJsonRequest* request);
};

extern cl_TURIPserverSerial TURIPserverSerial;

#endif
