#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <string.h>

#include "button.h"

int i2c_fd1;
int i2c_fd2;

#define GPIO4 4
#define GPIO5 5

int pin1 = GPIO4;
int pin2 = GPIO5;

unsigned char Slave_Addr1=0x0F;
unsigned char Slave_Addr2=0x0D;


void ctrl_c(int sig)
{
	close(i2c_fd1);
	close(i2c_fd1);

	exit(-1);
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  	MYSQL *con = mysql_init(NULL);
  	signal(SIGINT, ctrl_c);
	int i = 0;
  	int j = 0;
	unsigned char ret1[15], ret2[30];
	char ret3[30];
  	char coordinates[30];
	char *strptr1;
	char *strptr2;
  	char latitude[15];
  	char longitude[15];
  	char temperatura[15];

  	int enable;
  	int is_locked;
  	int transport_id = 1;
  	char q[200];

	//Setando arquivos e enderecos para uso do i2c
	i2c_fd1 = open("/dev/i2c-1", O_RDWR);
	ioctl(i2c_fd1, I2C_SLAVE, Slave_Addr1);
	i2c_fd2 = open("/dev/i2c-1", O_RDWR);
	ioctl(i2c_fd2, I2C_SLAVE, Slave_Addr2);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "root", "root",
          "transorg", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS transorg (Date DATETIME, Latitute TEXT, Longitude TEXT, Temperatura TEXT, Is_Locked INT, Enable INT, Transporte_ID INT)")) {
      finish_with_error(con);
  }
	//sleep para normalizacao dos sistemas
	sleep(30);

	//Pegando os valores de enable
	Button(&enable, pin1);

	//Pegando os valores de is_locked
	Button(&is_locked, pin2);

	//Recebendo dados de temperatura via i2c
	read(i2c_fd1, ret1, 15);
	printf("Recebido1: %s\n", ret1);
	for (i = 0; i < 15; i ++){
		if (ret1[i] == 255)
			ret1[i] = '\0';}

	sleep(1);

	//Recebendo dados das Coordenadas
	read(i2c_fd2, ret2, 30);
	printf("Recebido2: %s\n", ret2);
	for (i = 0; i< 30; i++){
		if(ret2[i] == 255)
			ret2[i] = '\0';
		//printf("%d ", ret2[i]);
	}


	strcpy(ret3, ret2);
	strcpy(temperatura, ret1);
	printf("%s\n", temperatura);
	printf("%s\n", ret2);
	printf("%s\n", ret3);

	strptr1 = strtok(ret3, " ");
	strptr2 = strtok(NULL, " ");
	printf("%s\n", strptr1);
	printf("%s\n", strptr2);

	strcpy(latitude, strptr1);
	strcpy(longitude, strptr2);

  sprintf(q, "INSERT INTO transorg Values(CURRENT_TIMESTAMP, '%s', '%s', '%s', %d,  %d, %d)", latitude, longitude, temperatura, is_locked, enable, transport_id);
  if (mysql_query(con, q)) {
      finish_with_error(con);
  }
  
	close(i2c_fd1);
	close(i2c_fd2);
  	mysql_close(con);
  	exit(0);
}
