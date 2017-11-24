#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <string.h>
#include "comunication_i2c.h"

void i2c_init(char *slave_addr, int *i2c_fd){

  *i2c_fd = open("/dev/i2c-1", O_RDWR); //abre o arquivo que permite a comunicacao i2c
  ioctl(*i2c_fd, I2C_SLAVE, *slave_addr); //RPI como master e outros como slave

}

void i2c_data(int *i2c_fd, char *recebido){

  unsigned char  b[30];
  

  read(*i2c_fd, b, 30);

 strcpy(recebido, b);
}

void i2c_close(int *i2c_fd){

  close(*i2c_fd);
}
