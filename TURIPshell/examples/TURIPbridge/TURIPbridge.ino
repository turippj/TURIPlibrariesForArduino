#include <SPI.h>
#include <TURIPcommon.h>
#include <TURIPclient.h>
#include <TURIPclientSPI.h>
#include <TURIPserver.h>
#include <TURIPshell.h>

const uint32_t TURIP_MODEL = 0x00000001;
const uint32_t TURIP_SERIAL = 0x00000000;

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
      Serial.println(TURIPshell(strBuf));
      strBuf = "";
    }else{
      strBuf += c;
    }
  }
}
