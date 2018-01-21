#include <TURIPserver.h>
#include <TURIPshell.h>

const uint32_t TURIP_MODEL = 0x371b;
const uint32_t TURIP_SERIAL = 0x0;

const int PIN_AI1 = 0;
const int PIN_AI2 = 1;
const int PIN_AO1 = 5;
const int PIN_AO2 = 6;
const int PIN_DI1 = 7;
const int PIN_DI2 = 8;
const int PIN_DO1 = 3;
const int PIN_DO2 = 4;

TURIPport* dIn1;
TURIPport* dIN2;
TURIPport* dOut1;
TURIPport* dOut2;
TURIPport* aIn1;
TURIPport* aIn2;
TURIPport* aOut1;
TURIPport* aOut2;

void setup(){
  Serial.begin(9600);

  pinMode(PIN_DI1, INPUT);
  pinMode(PIN_DI2, INPUT);
  pinMode(PIN_DO1, OUTPUT);
  pinMode(PIN_DO2, OUTPUT);
  analogWrite(PIN_AO1, 0);
  analogWrite(PIN_AO2, 0);

  TURIPserver.begin(TURIP_MODEL, TURIP_SERIAL);
  dIn1 = TURIPserver.newPort(1);
  dIN2 = TURIPserver.newPort(2);
  dOut1 = TURIPserver.newPort(3);
  dOut2 = TURIPserver.newPort(4);
  aIn1 = TURIPserver.newPort(5);
  aIn2 = TURIPserver.newPort(6);
  aOut1 = TURIPserver.newPort(7);
  aOut2 = TURIPserver.newPort(8);

  dIn1->writeUint16(0);
  dIN2->writeUint16(0);
  dOut1->writeUint16(0);
  dOut2->writeUint16(0);
  aIn1->writeUint16(0);
  aIn2->writeUint16(0);
  aOut1->writeUint16(0);
  aOut2->writeUint16(0);

  aIn1->preTransmitFunc = setValues;
  aIn2->preTransmitFunc = setValues;
  aOut1->postReceiveFunc = setValues;
  aOut2->postReceiveFunc = setValues;
}

void loop(){
  serialEvent();
}

void serialEvent(){
  static String strBuf;
  while(Serial.available()){
    char c = Serial.read();
    if(c == 0x0a){  // 0x0a: LF
      strBuf.trim();
      String response = TURIPshell(strBuf);
      if (response.length() != 0) Serial.println(response);
      strBuf = "";
    }else{
      strBuf += c;
    }
  }
}

void setValues(){
  analogWrite(PIN_AO1, aOut1->readUint16());
  analogWrite(PIN_AO1, aOut2->readUint16());
  aIn1->writeUint16(analogRead(PIN_AI1));
  aIn2->writeUint16(analogRead(PIN_AI2));
}
