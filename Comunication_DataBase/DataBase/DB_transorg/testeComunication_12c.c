

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <string.h>

int i2c_fd1;
int i2c_fd2;
void ctrl_c(int sig)
{
	close(i2c_fd1);
	close(i2c_fd2);
	exit(-1);
}



int main(){

	unsigned char user_input=1, ret1[15], ret2[30], slave_addr1=0x0F, slave_addr2 = 0x0D;
	signal(SIGINT, ctrl_c);
	char str1[30];
	char *str2;
	int i, j;
	i2c_fd1 = open("/dev/i2c-1", O_RDWR);
	ioctl(i2c_fd1, I2C_SLAVE, slave_addr1);
	i2c_fd2 = open("/dev/i2c-1", O_RDWR);
	ioctl(i2c_fd2, I2C_SLAVE, slave_addr2);

	while(user_input!=0)
	{
		puts("Digite um numero entre 1 e 5");
		puts("ou digite 0 para terminar o programa. ");
		scanf("%d", &user_input);

		if((user_input<0) || (user_input>5))
			puts("Valor invalido");
		else if(user_input>0)
		{

		read(i2c_fd1, ret1, 15);
		printf("Recebido1: %s\n", ret1);
		for (i = 0; i < 15; i ++){
			if (ret1[i] == 255)
				ret1[i] = '\0';}
		sleep(1);
		read(i2c_fd2, ret2, 30);
		printf("Recebido2: %s\n", ret2);
		for (i = 0; i< 30; i++){
			if(ret2[i] == 255)
				ret2[i] = '\0';
			printf("%d ", ret2[i]);
			}
		}
		puts("");
	}


	for (j = 0; ret2[j]; j ++){
		str1[j] = ret2[j];
		printf("%d ", str1[j]);
		}
	//str1 = strtok(ret2, ",");
	//str2 = strtok(NULL, ",");
	//printf("Str1: %s\nStr2: %s\n", str1, str2);
	close(i2c_fd1);
	close(i2c_fd2);

	return 0;


}
