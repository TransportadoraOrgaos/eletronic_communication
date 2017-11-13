#include <my_global.h>
#include <mysql.h>
#include "temperature.h"

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void TableTemperature(float temperature)
{
  MYSQL *con = mysql_init(NULL);

  char string[15] = {"Temperature"};
  char q[200];

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

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS temperature (TEMPERATURE FLOAT)")) {
      finish_with_error(con);
  }
 
  sprintf(q, "INSERT INTO temperature Values('%s', %.2f)",string, temperature);
  if (mysql_query(con, q)) {
      finish_with_error(con);}

  mysql_close(con);
  exit(0);
}
