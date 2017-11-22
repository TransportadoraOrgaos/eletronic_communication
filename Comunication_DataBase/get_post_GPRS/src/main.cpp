#include <Arduino.h>
#include <ArduinoJson.h>
#include <Http.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <stdlib.h>
#include <string.h>

#define Slave_Address 0x0F

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

unsigned int RX_PIN = 7;
unsigned int TX_PIN = 8;
unsigned int RST_PIN = 12;
int receive = 0;
char lat[12];
char lon[12];
char coordinates[30];
unsigned char coord[30];
HTTP http(9600, RX_PIN, TX_PIN, RST_PIN);

SoftwareSerial mySerial(TX_PIN, RX_PIN);
DFRobot_SIM808 sim808(&mySerial);
void getGPS();
void receiveData(int num);
void sendData();
void print(const __FlashStringHelper *message, int code = -1);
bool shouldTrackTimeEntry();
void trackTimeEntry();



// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting!");
  Wire.begin(Slave_Address);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

// the loop routine runs over and over again forever:
void loop() {
	if (receive == 1){
			 while(!sim808.init())//turn on gps
  				{
      				Serial.print("Sim808 init error\r\n");
      					delay(1000);
 				 }
			delay(3000);	
		getGPS(); //get coordinates
		sim808.detachGPS(); //turn off gps
		receive = 0;
  }

	if (receive == 2){
  if (shouldTrackTimeEntry()) trackTimeEntry();
	
	receive = 0;
	}
}


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

  sprintf(body, "{\"is_locked\": 1, \"date\": \"17/11/2017\",\"temperature\": 2.0, \"longitude\": \"%s\", \"enable\":1, \"latitude\": \"-16.0090\", \"transport_id\": 50}", lat);

  result = http.post("http://transports-rest-api.herokuapp.com/report/50", body, response);
  print(F("HTTP POST: "), result);
  if (result == SUCCESS) {
    Serial.println(response);
    StaticJsonBuffer<32> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    lastRunTime = millis();
    waitForRunTime = root["waitForRunTime"];

    print(F("Last run time: "), lastRunTime);
    print(F("Next post in: "), waitForRunTime);
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



void getGPS(){

	float la = sim808.GPSdata.lat;
	float lo = sim808.GPSdata.lon;

	dtostrf(la, 4, 6, lat); //put float value of la into char array of lat. 4 = number of digits before decimal sign. 6 = number of digits after the decimal sign.
  	dtostrf(lo, 4, 6, lon); //put float value of lo into char array of lon
  	
  	strcpy(coordinates, lat);
  
  	strcat(coordinates, " ");
  
  	strcat(coordinates, lon);

  	for(int i = 0; coordinates[i]!='\0'; i++){
  	  coord[i] = coordinates[i];
  	  }

}

void receiveData(int num){

	receive = Wire.read();
}

void sendData(){
	Wire.write(coord,30);
}
