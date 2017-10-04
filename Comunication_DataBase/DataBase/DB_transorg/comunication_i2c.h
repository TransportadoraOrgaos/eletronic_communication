/*Programa utilizado para criar as funções de comunicacao i2c
 *Funcoes usadas no programa estarao especificadas em comunication_i2c.c
 *********************************************************************/

 #ifndef COMUNICATIONI2C_H
 #define COMUNICATIONI2C_H


 void i2c_init(char *slave_addr, int *i2c_fd);
 void i2c_data(int *i2c_fd, float *temperatura);
 void i2c_close(int *i2c_fd);

 #endif
