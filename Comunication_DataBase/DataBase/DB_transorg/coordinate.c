#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

float TableCoordinate(float lat, float lon)
{
  MYSQL *con = mysql_init(NULL);

  char string[15] = {"Coordinates"};
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

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS coordinate (Name TEXT,lat FLOAT, Lon FLOAT)")) {
      finish_with_error(con);
  }
  sprintf(q, "INSERT INTO coordinate Values('%s', %.2f, %.2f)",  string, lat, lon);
  if (mysql_query(con, q)) 
      finish_with_error(con);


  mysql_close(con);
  exit(0);
}
