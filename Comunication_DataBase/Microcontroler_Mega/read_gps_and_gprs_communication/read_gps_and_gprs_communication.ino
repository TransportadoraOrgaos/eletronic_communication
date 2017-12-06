#include <Arduino.h>
#include <ArduinoJson.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <Http.h>
#include <Wire.h>
#include <string.h>

#define PIN_TX 10
#define PIN_RX 11

void(* resetFunc) (void) = 0;

char lat[12];
char lon[12];
char coordinates[30];
unsigned char coord[30];
unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;
int i = 0;
int j = 0;
int recebe[200] = {230};
char recebechar[200];
 
unsigned int RST_PIN = 12;
HTTP http(9600, PIN_RX, PIN_TX, RST_PIN);

SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,

void print(const __FlashStringHelper *message, int code = -1){
  if (code != -1){
    Serial.print(message);
    Serial.println(code);
  }
  else {
    Serial.println(message);
  }
}

bool shouldTrackTimeEntry(){
  unsigned long elapsedTime = millis() - lastRunTime;
  print(F("Elapsed time: "), elapsedTime);
  return elapsedTime >= waitForRunTime;
}

void trackTimeEntry(){
  char response[300];
  char body[300];
  Result result;
  
  print(F("Cofigure bearer: "), http.configureBearer("tim.br"));
  result = http.connect();
  print(F("HTTP connect: "), result);
  Serial.println(recebechar);
  sprintf(body, recebechar);
  result = http.post("http://transports-rest-api.herokuapp.com/createreport", body, response);
  print(F("HTTP POST: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<32> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    
    print(F("Last run time: "), lastRunTime);  
  }
  print(F("HTTP disconnect: "), http.disconnect());
}


void receiveEvent(int howMany){
  Serial.println("Começando escrever...");
  while(0 != Wire.available()){
    recebe[i] = Wire.read();
    if(recebe[i] == 29){
      recebe[i] = 32;
    }
    recebechar[i] = (char)recebe[i];
    Serial.print(recebechar[i]);
    i++;
  }

  Serial.print(recebechar[0]);
  delay(500);
  j++;

  if(j==8){
    i = 0;
    j = 0;
  
  }
  
}


void getGPS();
void sendData(){
  Wire.write(coord,30);
  Serial.println("Mandando Coordenadas");
}
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  Wire.begin(0x0D);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);
  while(!sim808.init())
  {
      delay(1000);
  }
}

void loop(){
   getGPS();
   Serial.println("GetGPS passou!");
   sim808.detachGPS();
   Serial.println("GPS desligado!");
   Serial.println("Delay de 1 minuto iniciado.");
   delay(90000);
   Serial.println(recebe[0]);
   if(recebe[0] == 32){
   if (shouldTrackTimeEntry()) trackTimeEntry();
   Serial.println("Resetando em 3..");
   delay(3000);
   resetFunc();}
   
    Serial.println("Resetando o arduino");
    resetFunc();
   
}

void getGPS(){ 
  while(!sim808.attachGPS())
  {
    Serial.println("Open the GPS power failure");
    delay(1000);
  }
  delay(1000);

  Serial.println("Open the GPS power success");
    
  while(!sim808.getGPS()){}
  Serial.println("Get GPS adquirido");
  float la = sim808.GPSdata.lat;
  Serial.println(la);
  float lo = sim808.GPSdata.lon;
  Serial.println(lo);
  
  
  dtostrf(la, 4, 6, lat);
  dtostrf(lo, 4, 6, lon); 
  strcpy(coordinates, lat);
  strcat(coordinates, " ");
  strcat(coordinates, lon);
  for(int i=0; coordinates[i]!='\0'; i++){
    coord[i] = coordinates[i];
  }
  Serial.println("coord");
  }

