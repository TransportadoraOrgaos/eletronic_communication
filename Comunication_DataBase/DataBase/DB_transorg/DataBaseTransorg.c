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


/*Functions
i2c_close(int fd);
i2c_init(char, int);
i2c_data(int, char);*/
#include "comunication_i2c.h"
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
	i2c_close(&i2c_fd1);
	i2c_close(&i2c_fd1);

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
  char coordinates[30];

  char latitude[15] = "-16.45";
  char longitude[15] = "-48.567834";
  char temperatura[10] = "5.34";

  int enable;
  int is_locked;
  int transport_id = 50;
  char date[15];
  char q[200];

  i2c_init(&Slave_Addr1, &i2c_fd1);
  i2c_init(&Slave_Addr2, &i2c_fd2);

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

	//Pegando os valores de enable

	Button(&enable, pin1);

	//Pegando os valores de is_locked
	Button(&is_locked, pin2);


  sprintf(q, "INSERT INTO transorg Values(CURRENT_TIMESTAMP, '%s', '%s', '%s', %d,  %d, %d)", latitude, longitude, temperatura, is_locked, enable, transport_id);
  if (mysql_query(con, q)) {
      finish_with_error(con);
  }
  

  mysql_close(con);
  exit(0);
}
