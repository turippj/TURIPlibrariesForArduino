#include <TURIPserver.h>
#include <TURIPclientSPI.h>
#include <TURIPshell.h>

void setup() {
  Serial.begin(9600);
  
  TURIPclientSPI.addPin(6);
  TURIPclientSPI.addPin(7);
  TURIPclientSPI.addPin(8);
  TURIPclientSPI.addPin(9);
  TURIPclientSPI.addPin(10);
  TURIPclientSPI.begin();
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

