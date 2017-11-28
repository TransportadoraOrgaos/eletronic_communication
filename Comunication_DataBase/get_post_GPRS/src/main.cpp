#include <Arduino.h>
#include <ArduinoJson.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <Http.h>
#include <Wire.h>
#include <string.h>

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

unsigned int RX_PIN = 7;
unsigned int TX_PIN = 8;
unsigned int RST_PIN = 12;
HTTP http(9600, RX_PIN, TX_PIN, RST_PIN);

char lat[12];
char lon[12];
char coordinates[30];
unsigned char coord[30];

SoftwareSerial mySerial(TX_PIN,RX_PIN);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,


void getGPS()
{ 
  while(!sim808.init())
  {
      Serial.print("Sim808 init error\r\n");
      delay(1000);
  }

  while(!sim808.attachGPS())
  {
    Serial.println("Open the GPS power failure");
    delay(1000);
  }
  delay(3000);

  Serial.println("Open the GPS power success");
    
  while(!sim808.getGPS())
  {
    
  }

  float la = sim808.GPSdata.lat;
  float lo = sim808.GPSdata.lon;
  

  dtostrf(la, 4, 6, lat); //put float value of la into char array of lat. 4 = number of digits before decimal sign. 6 = number of digits after the decimal sign.
  dtostrf(lo, 4, 6, lon); //put float value of lo into char array of lon
  
  strcpy(coordinates, lat);
  Serial.println(coordinates);
  strcat(coordinates, " ");
  Serial.println(coordinates);
  strcat(coordinates, lon);
  Serial.println(coordinates);
  for(int i = 0; coordinates[i]!='\0'; i++){
    coord[i] = coordinates[i];
    }
    Serial.print("Coordinates: ");
    Serial.println(coordinates);
  sim808.detachGPS();
  
}



int i = 0;
int j = 0;
int recebe[200];
char recebechar[200];

// functions
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
  // This calculation uses the max value the unsigned long can store as key. Remember when a negative number
  // is assigned or the maximun is exceeded, then the module is applied to that value.
  unsigned long elapsedTime = millis() - lastRunTime;
  print(F("Elapsed time: "), elapsedTime);
  return elapsedTime >= waitForRunTime;
}

void trackTimeEntry(){

  char response[300];
  char body[150];
  Result result;
  
  print(F("Cofigure bearer: "), http.configureBearer("tim.br"));
  result = http.connect();
  print(F("HTTP connect: "), result);

  //sprintf(body, "{\"is_locked\": 1, \"date\": \"17/11/2017\", \"temperature\": 2.0, \"longitude\": \"-48.5134\", \"enable\":1, \"latitude\": \"-16.0090\", \"transport_id\": 50}");
  sprintf(body, recebechar);
  result = http.post("http://transports-rest-api.herokuapp.com/report/50", body, response);
  print(F("HTTP POST: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<32> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    //waitForRunTime = root["waitForRunTime"];
    
    print(F("Last run time: "), lastRunTime);
    print(F("Next post in: "), waitForRunTime);
    print(F("Delay of 5 min starting now "));
    delay(300000);  
  }

/*  result = http.get("your.domain/api/timing", response);
  print(F("HTTP GET: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<32> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    waitForRunTime = root["waitForRunTime"];

    print(F("Last run time: "), lastRunTime);
    print(F("Next post in: "), waitForRunTime);
  }*/

  print(F("HTTP disconnect: "), http.disconnect());
}

void receiveEvent(int howMany)
{
  while(0 != Wire.available()) // loop through all but the last
  {
    //char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
    
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


void sendData(){
  Wire.write(coord,30);
}


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600); 
  Wire.begin(0x0F);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);

  while(!Serial);
  Serial.println("Starting!");
}

// the loop routine runs over and over again forever:
void loop() {
  getGPS();
  Serial.println(recebechar);
  delay(30000);
  if (shouldTrackTimeEntry()) trackTimeEntry();
}




