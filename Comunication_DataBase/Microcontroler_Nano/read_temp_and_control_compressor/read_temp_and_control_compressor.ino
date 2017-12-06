#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 3
char b[15];
 
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
float tempC;

DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

void requestEvent() {
  Wire.write(b,15); // Resposta com uma mensagem de 15 bytes
}

void setup() {
  Wire.begin(0x0F);             // Registra o barramento i2c com o endereço 0x0F
  Wire.onRequest(requestEvent); // Registra o evento
  Serial.begin(9600);
  
  pinMode(9, OUTPUT);
  
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  Serial.println();
  
    
}

void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop() {
  sensors.requestTemperatures();
  tempC = sensors.getTempC(sensor1);
  Serial.println(tempC);
  delay(1000);
  dtostrf(tempC, 4, 6, b);
  Serial.println(b);
  Serial.print("Imprimindo B: ");
  Serial.println(b);
  
  if(tempC >= 4){
    digitalWrite(9,HIGH);
  }
  if(tempC <= 2){
    digitalWrite(9,LOW);
  }
}