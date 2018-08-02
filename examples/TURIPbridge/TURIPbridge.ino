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
  static String strBuf;
  while(Serial.available()){
    char c = Serial.read();
    if(c == 0x0a){  // 0x0a: LF
      strBuf.trim();
      if(strBuf.length() > 0){
          char response[128];
          char request[64];
          strBuf.toCharArray(request, 64);
          TURIPshell(request, response, 127);
          Serial.println(response);
      }
      strBuf = "";
    }else{
      strBuf += c;
    }
  }
}
