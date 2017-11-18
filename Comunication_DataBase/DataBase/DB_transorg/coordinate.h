#ifndef COORDINATE_H
#define COORDINATE_H
#include <mysql.h>
#include <my_global.h>
void TableCoordinate(float lat, float lon);
void finish_with_error(MYSQL *con);

#endif
