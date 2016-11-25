#ifndef TURIP_ON_UART_SERVER_H
#define TURIP_ON_UART_SERVER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdlib.h>
#include <string.h>
#include "TURIPserver.h"


class cl_TURIPonUARTserver{
public:
  void begin();
  /*
  初期化動作を行う(UART通信の開始)
  */

  void communicate();
  /*
  clientの要求を受け付けてから返答を返すまでやるやつ
  */
};

extern cl_TURIPonUARTserver TURIPonUARTserver;

#endif
