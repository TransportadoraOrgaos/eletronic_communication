/*******************************************************************
 *Programa usado para criar uma tabela dentro do banco de dados transorg
 *Precisa ser executado para se adicionar os dados necessarios
 *Dados: Temperatura, pressao*, latitude, longitude, is_locked
 *E necessario adicionar dados na tabela
 *CODIGO A SER MELHORADO
 *Tabela: transorg
 *******************************************************************/


#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);
  int i = 0;
  int j = 0;
char string[5][15] = {"Temperatura", "Pressao", "Latitude", "Longitude", "Is_Locked"};
  char q[200];
  float temp = 1.23;
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

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS transorg (ID INT, Name TEXT, vari FLOAT)")) {
      finish_with_error(con);
  }
  while(j < 10){
    if (i == 5)
        i = 0;
  sprintf(q, "INSERT INTO transorg Values(%d, '%s', %.2f)", i, string[i], temp);
  if (mysql_query(con, q)) {
      finish_with_error(con);
  }
  i++;
  j++;
  }

  mysql_close(con);
  exit(0);
}
