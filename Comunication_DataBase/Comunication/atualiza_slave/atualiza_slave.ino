#include <Wire.h>

float variavel_float = -1.1;

unsigned char b[4];
unsigned int aux;

void setup() {
  delay(500);                           // Espera o escravo se estabilizar
  Serial.begin(9600);
  Wire.begin(0x0F);
  Wire.onRequest(sendData);

  // Para variáveis float positivas
  // Ajustando o número antes da vírgula
  aux = (unsigned int) variavel_float;  // aux = 46689, Pega somente a parte inteira da variável float (0 - 65536)
  b[1] = aux;                          // byte2 = 0B01100001, pega apenas os primeros 8 bits
  b[0] = (aux>>8);                     // byte1 = 0B10110110, pega os 8 ultimos bits
  
  // Ajustando o número depois da vírgula
  variavel_float -= aux;                // Deixa apenas o número depois da vírgula
  variavel_float *= 10000;              // Multiplica por 10k para pegar 4 dígitos após a vírgula
  aux = (unsigned int) variavel_float;  // Pega somente o valor antes da vírgula
  b[3] = aux;                          // byte2 = 0B00101110, pega apenas os primeros 8 bits
  b[2] = (aux>>8);                     // byte1 = 0B00100010, pega os 8 ultimos bits

}
 
 void sendData(){ 
  Wire.write(b, 4);                  
}

void loop() {
  delay(1000);
}
