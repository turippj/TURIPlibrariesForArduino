#include <TURIPclientSPI.h>
#include <TURIPserver.h>
#include <TURIPshell.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "EEPROM.h"
#include "BluetoothSerial.h"

#define DEBUG

uint32_t TURIP_MODEL = 0x0;
uint32_t TURIP_SERIAL = 0x0;

TURIPport* portSsid;
TURIPport* portPassword;

BluetoothSerial SerialBT;

boolean wifiSettingIsChanged = false;
boolean webserverIsRunning = false;

const size_t EEPROM_SIZE = 98;
union {
  struct {
    char ssid[33];
    char pass[65];
  } data;
  uint8_t address[EEPROM_SIZE];
} rom;

WiFiServer server(80);

void setup() {
  TURIP_MODEL = (uint32_t)(ESP.getEfuseMac() >> 32);
  TURIP_SERIAL = (uint32_t)ESP.getEfuseMac();
//  Serial.begin(115200);
  String string_btName = "TPOT-" + String(TURIP_MODEL, HEX) + String(TURIP_SERIAL, HEX);
  char btName[64];
  string_btName.toCharArray(btName, 64);
  Serial.begin(9600);
  SerialBT.begin(btName);
  EEPROM.begin(EEPROM_SIZE);

  TURIPserver.begin(TURIP_MODEL, TURIP_SERIAL);
  portSsid = TURIPserver.newPort(1);
  portPassword = TURIPserver.newPort(2);
  portSsid->writeString(String("****"));
  portPassword->writeString(String("****"));
  portSsid->postReceiveFunc = setWifiSsid;
  portPassword->postReceiveFunc = setWifiPasswd;

  TURIPclientSPI.addPin(25);
  TURIPclientSPI.addPin(33);
  TURIPclientSPI.addPin(26);
  TURIPclientSPI.addPin(27);
  TURIPclientSPI.begin();

  readWifiSettings();

//  Serial.println(rom.data.ssid);
//  Serial.println(rom.data.pass);

  if(rom.data.ssid[0] != '\0' && rom.data.pass[0] != '\0'){
    WiFi.begin(rom.data.ssid, rom.data.pass);
  }
}

void readWifiSettings(){
  for(int i = 0; i < EEPROM_SIZE; i++){
    rom.address[i] = EEPROM.read(i);
  }
  rom.data.ssid[32] = '\0';
  rom.data.pass[64] = '\0';
}

void regWifiSettings(){
  for(int i = 0; i < EEPROM_SIZE; i++){
    EEPROM.write(i, rom.address[i]);
  }
  EEPROM.commit();
  for(int i = 0; i < EEPROM_SIZE; i++){
    Serial.print((char)EEPROM.read(i));
  }
}

void loop() {
  if(wifiSettingIsChanged){
    regWifiSettings();
    WiFi.begin(rom.data.ssid, rom.data.pass);
    wifiSettingIsChanged = false;
  }
  if(WiFi.status() == WL_CONNECTED){
    if(webserverIsRunning){
      webserver();
    } else {
      server.begin();
      webserverIsRunning = true;
    }
  }

  serialEvent();
  btEvent();
}

void setWifiSsid(){
  portSsid->readString().toCharArray(rom.data.ssid, 32);
  portSsid->writeString(String("****"));
  wifiSettingIsChanged = true;
}

void setWifiPasswd(){
  portPassword->readString().toCharArray(rom.data.pass, 64);
  portPassword->writeString(String("****"));
  wifiSettingIsChanged = true;
}

void webserver(){
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

#ifdef DEBUG
  Serial.println("new client");
#endif

  int status = 500;

  while(!client.available()){
    delay(1);
  }

  String request = client.readString();
#ifdef DEBUG
  Serial.println("request: ");
  Serial.println(request);
#endif
  int indexofPath = request.indexOf("/");
  int indexofPost = request.indexOf("\x0d\x0a\x0d\x0a"); // double CRLF
  String method = request.substring(0,indexofPath - 1);
  String post = request.substring(indexofPost, request.length());
  method.toLowerCase();
  method.trim();
  post.trim();
  if(method.equals("get")){
#ifdef DEBUG
    Serial.println("GET method");
#endif
    status = 200;
  }else if(method.equals("post")){
#ifdef DEBUG
    Serial.println("POST method");
    Serial.println(indexofPost);
    Serial.println(request.length());
    Serial.println(post);
#endif
    status = 200;
  }else {
#ifdef DEBUG
    Serial.println("invalid method");
#endif
    status = 400;
  }
  String path = request.substring(indexofPath, request.indexOf(' ', indexofPath));
  path.toLowerCase();
  path.trim();
#ifdef DEBUG
  Serial.print("path: ");
  Serial.println(path);
#endif

  String body;

  // TURIP query.
  int pathRootIndex = path.indexOf("turip");
  if(pathRootIndex != -1){
    path.remove(0, 6);
    Serial.println(path);
    char request[64];
    char response[128];
    path.toCharArray(request, 64);
    if(method.equals("get")){
      TURIPshell(request, response, 128);
      body += response;
    }else if(method.equals("post")){
      path += String(" " + post);
      path.toCharArray(request, 64);
      TURIPshell(request, response, 128);
      body += response;
    }
  }else{
#ifdef DEBUG
    Serial.println("Invalid path.");
#endif
    status = 404;
  }

  client.flush();
  String response = "HTTP/1.1";
  if(status == 200){
    response += " 200 OK\n";
    response += "Content-Type: application/json\n";
    response += "Access-Control-Allow-Origin: *\n\n";
    response += body;
    response += "\n";
  }else if(status == 400){
    response += " 400 Bad Request\n";
  }else if(status == 403){
    response += " 403 Forbidden\n";
  }else if(status == 404){
    response += " 404 Not Found\n";
  }else{
    response += " 500 Internal Server Error\n";
  }
  client.print(response);

  delay(1);
#ifdef DEBUG
  Serial.println("Client disonnected");
#endif
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

void btEvent(){
  static String strBuf;
  while(SerialBT.available()){
    char c = SerialBT.read();
    if(c == 0x0a){  // 0x0a: LF
      strBuf.trim();
      if(strBuf.length() > 0){
        char response[128];
        char request[64];
        strBuf.toCharArray(request, 64);
        TURIPshell(request, response, 127);
        SerialBT.println(response);
      }
      strBuf = "";
    }else{
      strBuf += c;
    }
  }
}
