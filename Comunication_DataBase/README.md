# PI2
Pasta criada para codigos relacionados a comunicacao entre a Raspberry Pi e os Microcontroladores e o envio das variaveis utilizadas na Raspberry para um banco de dados 'transorg'.

# libraries
Contem todas as bibliotecas usadas nos trabalho, tanto as do microcontrolador, e a usada na RPI para o uso dos GPIO como entradas.

# Microcontroler_Mega
Codigo fonte usado no Mega

# Microcontroler_Nano
Codigo Fonte usado no Nano

# Raspberry_Code
Codigos usados no RPI, para criacao do banco de dados, recebimentos das variaveis dos microcontroladores, (DataBaseTransorg)

Codigo usado para ler o banco de dados, e formar o json e enviar para o Mega e fazer o post usando o GPRS

# Instalation MySQL

	$ sudo apt-get install mysql-server
	The default user and password is 'root', 'root'.

Intall the lib of MySQL

	$ sudo apt-get install libmysqlclient-dev

Mas na RPI usada para testes nao houve como instalar a lib do MySQL, entao teve quer ser intalada a do MariaDB

	$ sudo apt-get install libmariadbclient-dev

# To Compile 

Using GCC and using libs of MySQL:

	$ gcc name_program.c -o name_program $(mysql_config --libs --cflags)

Refs:

	http://zetcode.com/databases/mysqltutorial/installation/

	http://zetcode.com/db/mysqlc/

# OBS.:
Foi usado o protoloco de comunicacao I2C entre o RPI e os microcontroladores.
