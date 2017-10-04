#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/i2c-dev.h>

#include "comunication_i2c.h"

void i2c_init(char *slave_addr, int *i2c_fd){

  *i2c_fd = open("/dev/i2c-1", O_RDWR); //abre o arquivo que permite a comunicacao i2c
  ioctl(*i2c_fd, I2C_SLAVE, *slave_addr); //RPI como master e outros como slave

}

void i2c_data(int *i2c_fd, float *temperatura){

  unsigned char  b[4];
  int aux;
  float temp;

  read(*i2c_fd, b, 4);

  aux = ((b[2] << 8) | b[3]);
  temp = (float)aux*0.0001;
  aux = ((b[0] << 8) | b[1]);
  temp+=aux;

  *temperatura = temp;
}

void i2c_close(int *i2c_fd){

  close(*i2c_fd);
}
