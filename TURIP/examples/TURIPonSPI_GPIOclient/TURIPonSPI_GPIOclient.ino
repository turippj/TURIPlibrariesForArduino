#include <TURIPclient.h>
#include <TURIPclientSPI.h>

TURIPdevice gpioModule;

void setup(){
  Serial.begin(9600);

  TURIPclientSPI.addPin(15);
  TURIPclientSPI.addPin(2);
  TURIPclientSPI.addPin(0);
  TURIPclientSPI.addPin(16);
  TURIPclientSPI.begin();

  TURIPclient.scan();

  if(gpioModule.attach(0x284f, 0x0) == TURIP_ERR){
    Serial.println("Not found.");
    while(1);
  }
  Serial.println("Connected.");
}

void loop(){
  Serial.print("analogIn 1: ");
  Serial.print(gpioModule.readUint16(7));
  Serial.print(", analogIn 2: ");
  Serial.print(gpioModule.readUint16(8));
}
