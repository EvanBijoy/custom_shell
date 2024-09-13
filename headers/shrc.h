#ifndef HOP_H
#define HOP_H
#include <stdbool.h>

void readMyshrc();
bool executeAlias(char *alias, char *home);
bool executeFunction(char *name, char *arg);

#endif