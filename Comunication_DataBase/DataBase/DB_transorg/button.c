
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include "button.h"


int arquive;
float timeSleep=0.5;
char buffer[3];
char path[35];




void Button (int *retorno, int pin)//*retorno, variavel para retornar os valores de enable e is_locked
{
	*retorno = 0;

	int i = 0;
   	int j = 0;

	// === Verifica se o pino ja foi exportado === //
	if(access_gpio(pin))
	{
		// === Export do Pino === //
		if (export_gpio(pin))
		{
		// === Configurando Direcao do Pino === //
			if(direction_gpio(pin, INPUT))
			{
				//=== Lendo o pino ===//
        			while(i < 5)
        			{
					if((value_in_gpio(pin, LOW) == LOW)) //Button press
					{
        	        			delay(timeSleep);
						printf("Button press %d \n", pin);
						printf("Enable ativado, caixa em transporte\n");
						*retorno=1;
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
				//return 2;
			}
		}
		else
		{
			printf("Ocorreu um problema na exportacao do pino!\n");
			//return 1;
		}

		// === Desvinculando o pino == //

		if(unexport_gpio(pin))
		{
			//return 0;
		}
		else
		{
			printf("Ocorreu um problema para finalizar a utilizacao do pino\n");
			//return 1;
		}
	}
	else
	{
		printf("Pino ja foi exportado! \n");
		delay(0.5);
		printf("Configurando as caracteristicas de utilizacao! \n");
		delay(0.5);

		if(direction_gpio(pin, INPUT))
                {
                                printf("GPIO%d configurada como INPUT! \n", pin);
                		delay(0.5);
				printf("Iniciando o processo de leitura\n");
				delay(0.5);
				//=== Lendo o pino ===//
                                while(j < 5)
                                {
                                        if((value_in_gpio(pin, LOW) == LOW)) //Button press
                                        {
                                                delay(timeSleep);
                                                printf("Caixa em transporte\n");
                                                *retorno=1;
                                        }
                                        else
                                        {
                                                delay(timeSleep);
                                                printf("Caixa em espera de transporte\n");
                                        }
                                }
		                // === Desvinculando o pino == //

                		if(unexport_gpio(pin))
                		{
                		        //return 0;
                		}
                		else
                		{
                		        printf("Ocorreu um problema para finalizar a utilizacao do pino\n");
                		        //return 1;
                		}

		}
		else
                {
                	printf("Ocorreu um problema na configuracao do pino como I/O\n");
                        //return 2;
               	}
	}
}




bool access_gpio(int pin)
{
	int ModAccess;
	snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
	if (access(path, 0) == -1) // Arquivo nao existe
	{
		return true;
	}
	else // Arquivo existe
	{
		return false;
	}
}

bool export_gpio(int pin)
{
        arquive = open ("/sys/class/gpio/export", O_WRONLY);
        if (arquive==-1)
        {
                printf("Arquivo abriu incorretamente\n");
                return false;
        }
        snprintf(buffer, 3, "%d", pin);
        if(write(arquive, buffer, 3) == -1)
        {
                close(arquive);
                return false;
        }

        close(arquive);

	return true;
}

bool direction_gpio(int pin, int direction)
{
	arquive=0;
        snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
        arquive = open (path, O_WRONLY);
	if (arquive==-1)
        {
		return false;
	}
        snprintf(buffer, 3, "%d", pin);
        if (write( arquive, ((direction == INPUT)?"in":"out"), 3 )==-1)
        {
                close(arquive);
                return false;
        }
        close(arquive);
	return true;
}

int value_in_gpio(int pin, int value)
{
	arquive=0;
	char retorno[3];
        snprintf(path, 35, "/sys/class/gpio/gpio%d/value", pin);
        arquive = open(path, O_RDONLY);
	//printf("Descritor do arquivo: %d \n", arquive);
        if (arquive == -1)
        {
                return false;
        }
        if (read(arquive, retorno, 3) == -1)
        {
                close(arquive);
                return false;
        }
	close(arquive);
	printf("Valor do pino %d: %c \n", pin, retorno[0]);

	return atoi(retorno);
}

bool unexport_gpio(int pin)
{
	arquive = open ("/sys/class/gpio/unexport", O_WRONLY);
        if (arquive==-1)
        {
                printf("Arquivo abriu incorretamente\n");
                return false;
        }
        if(write(arquive, buffer, 3) == -1)
        {
                close(arquive);
                return false;
        }
        return true;
}

void delay(float time)
{
	struct timespec t;
	int seg;
	seg = time;
	t.tv_sec = seg;
	t.tv_nsec = (time-seg)*1e9;
	nanosleep(&t, NULL);
}
