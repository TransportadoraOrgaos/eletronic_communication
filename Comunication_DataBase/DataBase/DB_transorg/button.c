
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
	printf("Valor do pino: %c \n", retorno[0]);

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
