#include <my_global.h>
#include <mysql.h>
#include "coordinate.h"
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void TableCoordinate(float lat, float lon)
{
  MYSQL *con = mysql_init(NULL);

  char string[15] = {"Coordinates"};
  char q[200];
  char lati[15];
  char longi[15];

  sprintf(lati, "%f", lat);
  sprintf(longi, "%f", lon);

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

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS coordinate (Name TEXT,Lat TEXT, Lon TEXT)")) {
      finish_with_error(con);
  }
  sprintf(q, "INSERT INTO coordinate Values('%s', '%s', '%s')",  string, lati, longi);
  if (mysql_query(con, q)) 
      finish_with_error(con);


  mysql_close(con);
  exit(0);
}
