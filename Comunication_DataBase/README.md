# PI2
Pasta criada para codigos relacionados a comunicacao entre a Raspberry Pi e o MSP430 e o envio das variaveis utilizadas na Raspberry para um banco de dados 'transorg'.

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
Dentro de DataBase, o creat_db_TransOrg.c, basta ser executado uma unica vez para que haja a criacao do banco de dados transorg.
O tableTrans.c cria a tabela dentro do banco de dados.

