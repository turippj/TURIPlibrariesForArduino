#include <TURIPclientSPI.h>
#include <TURIPserver.h>
#include <TURIPshell.h>

const uint32_t TURIP_MODEL = 0x792a;
const uint32_t TURIP_SERIAL = 0x0;

void setup() {
  Serial.begin(9600);

  TURIPclientSPI.addPin(6);
  TURIPclientSPI.addPin(7);
  TURIPclientSPI.addPin(8);
  TURIPclientSPI.addPin(9);
  TURIPclientSPI.addPin(10);
  TURIPclientSPI.begin();

  TURIPserver.begin(TURIP_MODEL, TURIP_SERIAL);
}

void loop() {
  serialEvent();
}

void serialEvent(){
  static char strBuf[128];
  static size_t bufSize = 0;
  while(Serial.available()){
    char c = Serial.read();
    if(c == 0x0a){  // 0x0a: LF
      strBuf[bufSize] = '\0';
      char response[128];
      TURIPshell(strBuf, response, 128);
      Serial.println(response);
      bufSize = 0;
    }else{
      strBuf[bufSize++] = c;
      if(bufSize == 128) bufSize = 0;
    }
  }
}
