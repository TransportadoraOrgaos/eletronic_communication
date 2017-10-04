#include <Wire.h>

#define SLAVE_ADDRESS 0x0F
#define LED  PD4

int number = 0;
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);   //apenas para mostrar na tela serial
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}

void receiveData(int byteCount) {
  Serial.print("receiveData");
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);
    }
}

void sendData() {    //so para enviar de volta e verificar se o valor recebido esta correto
  Wire.write(number);
}

