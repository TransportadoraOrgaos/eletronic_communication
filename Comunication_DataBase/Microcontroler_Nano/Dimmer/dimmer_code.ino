#define PINO_ZC 2
#define PINO_DIM 3

volatile long potencia = 0;  // 0 a 100 
 
void zeroCross()  {
  if (potencia>100) potencia=100;
  if (potencia<0) potencia=0;
  long t1 = 8200L * (100L - potencia) / 100L;      
  delayMicroseconds(t1);   
  digitalWrite(PINO_DIM, HIGH);  
  delayMicroseconds(6);      // t2
  digitalWrite(PINO_DIM, LOW);   
}
 
void setup() {
  pinMode(PINO_DIM, OUTPUT);
  attachInterrupt(0, zeroCross, RISING);
}
 
void loop() {
  for (byte i=2; i<90; i++) {
    potencia=i;
    delay(150);     
  }
  while(1)
  {
  luminosidade=90;
  }

}
 
