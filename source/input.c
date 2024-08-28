#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/commands.h"
#include "../headers/input.h"

void processInput(char *input, char *home)
{
    char *command;
    char *saveptr1;
    
    command = strtok_r(input, ";", &saveptr1);
    while (command != NULL)
    {
        executeCommand(command, home);
        command = strtok_r(NULL, ";", &saveptr1);
    }
}
