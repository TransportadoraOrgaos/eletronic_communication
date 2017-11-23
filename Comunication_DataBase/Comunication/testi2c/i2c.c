#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/i2c-dev.h>
int i2c_fd;
int main(){
  unsigned char rpi_addr = 0xDA, slave_addr = 0x0F;
  unsigned char byte1, byte2, byte3, byte4, b[4];
  int aux;
  float temp, temperatura;
  i2c_fd = open("/dev/i2c-1", O_RDWR); //abre o arquivo que permite a comunicacao i2c
  ioctl(i2c_fd, I2C_SLAVE, slave_addr); //RPI como master e outros como slave
  read(i2c_fd, b, 4);
for (int i = 0; i<4; i++){
  printf("%d\n", b[i]);}
  if(b[0] > 127){
	b[0] = 255 - b[0];
	b[1] = 255 - b[1];
	b[2] = 255 - b[2];
	b[3] = 255 - b[3];
	aux = ((b[2] << 8) | b[3]);
  	temp = (float)aux*0.0001;
  	aux = ((b[0] << 8 ) | b[1]);
  	temp+=aux;
	temp = temp * (-1);
}
else{
  aux = ((b[2] << 8) | b[3]);
  temp = (float)aux*0.0001;
  aux = ((b[0] << 8 ) | b[1]);
  temp+=aux;}
  temperatura = temp;
 printf("Temperatura: %.2f\n", temperatura);
  close(i2c_fd);
  return 0;
}
