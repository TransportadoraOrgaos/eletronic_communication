#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include "coordinate.h"

int i2c_fd;

void ctrl_c(int sig)
{
	close(i2c_fd);

	exit(-1);
}

int main(void)
{
	unsigned char user_input=1, slave_addr=0x0F;
	char ret[30];
	float lat;
	signal(SIGINT, ctrl_c);
	i2c_fd = open("/dev/i2c-1", O_RDWR);
	ioctl(i2c_fd, I2C_SLAVE, slave_addr);


	while(user_input!=0)
	{
		puts("Digite um numero entre 1 e 5");
		puts("ou digite 0 para terminar o programa. ");
		scanf("%d", &user_input);
		if((user_input<0) || (user_input>5))
			puts("Valor invalido");
		else if(user_input>0)
		{       
			write(i2c_fd, &user_input, 1);
			sleep(3);
			read(i2c_fd, ret, 30);

		printf("%s\n", 	ret);
		}
		puts("");
	}

	lat = atof(ret);

	TableCoordinate(2345.8907657, 12.123456);
	printf("%f",lat);
	close(i2c_fd);

}
