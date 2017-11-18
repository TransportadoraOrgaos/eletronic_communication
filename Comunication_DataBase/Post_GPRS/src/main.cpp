#include <Arduino.h>
#include <ArduinoJson.h>
#include <Http.h>

unsigned long lastRunTime = 0;
unsigned long waitForRunTime = 0;

unsigned int RX_PIN = 7;
unsigned int TX_PIN = 8;
unsigned int RST_PIN = 12;
HTTP http(9600, RX_PIN, TX_PIN, RST_PIN);


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

  sprintf(body, "{\"is_locked\": 1, \"date\": \"17/11/2017\", \"temperature\": 2.0, \"longitude\": \"-48.5134\", \"enable\":1, \"latitude\": \"-16.0090\", \"transport_id\": 50}");

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

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting!");
}

// the loop routine runs over and over again forever:
void loop() {
  if (shouldTrackTimeEntry()) trackTimeEntry();
}
