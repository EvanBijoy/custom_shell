#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/shrc.h"
#include "../headers/input.h"
#include "../headers/commands.h"

#define MAX_ALIAS_COUNT 256
#define MAX_ALIAS_SIZE 256
#define MAX_FUNCTION_COUNT 256
#define MAX_FUNCTION_SIZE 4096
#define BUFFERSIZE 4096

typedef struct Alias
{
    char alias[MAX_ALIAS_SIZE];
    char command[BUFFERSIZE];
} Alias;

Alias collection[MAX_ALIAS_COUNT];
int aliasCount = 0;

typedef struct Function
{
    char name[MAX_ALIAS_SIZE];
    char body[MAX_FUNCTION_SIZE];
} Function;

Function functions[MAX_FUNCTION_COUNT];
int functionCount = 0;

void readMyshrc()
{
    char shrc[BUFFERSIZE];
    if (getcwd(shrc, sizeof(shrc)) == NULL)
    {
        perror("getcwd() error");
        return;
    }
    strcat(shrc, "/myshrc");
    FILE *file = fopen(shrc, "r");
    if (file == NULL)
    {
        printf("Could not open myshrc file\n");
        return;
    }

    char line[BUFFERSIZE];
    int inFunction = 0;
    char functionBody[MAX_FUNCTION_SIZE] = "";
    char functionName[MAX_ALIAS_SIZE] = "";

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *commentStart = strchr(line, '#');
        if (commentStart != NULL)
        {
            *commentStart = '\0';
        }

        if (line[0] == '\0' || line[0] == '\n')
        {
            continue;
        }

        if (strstr(line, "alias") == line)
        {
            char aliasPart[MAX_ALIAS_SIZE];
            char commandPart[BUFFERSIZE];

            if (sscanf(line, "alias %s = %[^\n]", aliasPart, commandPart) == 2)
            {
                strncpy(collection[aliasCount].alias, aliasPart, MAX_ALIAS_SIZE);
                strncpy(collection[aliasCount].command, commandPart, BUFFERSIZE);
                aliasCount++;
            }
            else
            {
                printf("Invalid alias definition in myshrc.\n");
            }
        }

        else if (strncmp(line, "func", 4) == 0)
        {
            inFunction = 1;
            sscanf(line, "func %[^()]", functionName);
            functionBody[0] = '\0';
        }
        else if (inFunction && line[0] == '}')
        {
            inFunction = 0;
            strncpy(functions[functionCount].name, functionName, MAX_ALIAS_SIZE);
            strncpy(functions[functionCount].body, functionBody, MAX_FUNCTION_SIZE);
            functionCount++;
        }
        else if (inFunction)
        {
            strncat(functionBody, line, sizeof(functionBody) - strlen(functionBody) - 1);
        }
    }

    fclose(file);
}

bool executeAlias(char *alias, char *home)
{
    for (int i = 0; i < aliasCount; i++)
    {
        if (strcmp(alias, collection[i].alias) == 0)
        {
            printf("Executing alias command: %s\n", collection[i].command);
            processInput(collection[i].command, home);  // Execute the alias command
            return true;
        }
    }
    return false;
}

bool executeFunction(char *name, char *arg)
{
    for (int i = 0; i < functionCount; i++)
    {
        if (strcmp(name, functions[i].name) == 0)
        {
            printf("Executing function: %s with argument: %s\n", functions[i].name, arg ? arg : "(no arg)");
            char command[BUFFERSIZE];
            snprintf(command, sizeof(command), functions[i].body, arg ? arg : "");  // Fill function body with argument
            processInput(command, arg);  // Execute the function
            return true;
        }
    }
    return false;
}
