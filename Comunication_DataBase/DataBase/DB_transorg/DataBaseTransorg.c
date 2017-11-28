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

  int enable = 0;
  int is_locked = 1;
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

//Ativacao de enable atraves de uma chave externa como entrada no rpi
// === Verifica se o pino ja foi exportado === //
	if(access_gpio(pin1))
	{
		// === Export do Pino === //
		if (export_gpio(pin1))
		{
		// === Configurando Direcao do Pino === //
			if(direction_gpio(pin1, INPUT))
			{
				//=== Lendo o pino ===//
        			while(i < 5)
        			{
					if((value_in_gpio(pin1, LOW) == LOW)) //Button press
					{
        	        			delay(timeSleep);
						printf("Button press \n");
						printf("Enable ativado, caixa em transporte\n");
						enable=1;
                			}
					else
					{
						delay(timeSleep);
						printf("Button not press\n");
					}
					i++;
        			}
			}
			else
			{
				printf("Ocorreu um problema na configuracao do pino como I/O\n");
				return 2;
			}
		}
		else
		{
			printf("Ocorreu um problema na exportacao do pino!\n");
			return 1;
		}

		// === Desvinculando o pino == //

		if(unexport_gpio(pin1))
		{
			return 0;
		}
		else
		{
			printf("Ocorreu um problema para finalizar a utilizacao do pino\n");
			return 1;
		}
	}

//Ativacao de is_locked atraves de uma chave externa ao rpi
// === Verifica se o pino ja foi exportado === //
	if(access_gpio(pin2))
	{
		// === Export do Pino === //
		if (export_gpio(pin2))
		{
		// === Configurando Direcao do Pino === //
			if(direction_gpio(pin2, INPUT))
			{
				//=== Lendo o pino ===//
        			while(i < 5)
        			{
					if((value_in_gpio(pin2, LOW) == LOW)) //Button press
					{
        	        			delay(timeSleep);
						printf("Button press \n");
						printf("Enable ativado, caixa em transporte\n");
						is_locked=1;
                			}
					else
					{
						delay(timeSleep);
						printf("Button not press\n");
					}
					i++;
        			}
			}
			else
			{
				printf("Ocorreu um problema na configuracao do pino como I/O\n");
				return 2;
			}
		}
		else
		{
			printf("Ocorreu um problema na exportacao do pino!\n");
			return 1;
		}

		// === Desvinculando o pino == //

		if(unexport_gpio(pin2))
		{
			return 0;
		}
		else
		{
			printf("Ocorreu um problema para finalizar a utilizacao do pino\n");
			return 1;
		}
	}
   
  
  sprintf(q, "INSERT INTO transorg Values(CURRENT_TIMESTAMP, '%s', '%s', '%s', %d,  %d, %d)", latitude, longitude, temperatura, is_locked, enable, transport_id);
  if (mysql_query(con, q)) {
      finish_with_error(con);
  }
  

  mysql_close(con);
  exit(0);
}
