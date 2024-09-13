#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include "../headers/prompt.h"
#include "../headers/input.h"

#define BUFFERSIZE 4096

void showSpecialPrompt(char * home, long seconds, char * arg)
{
    char systemname[BUFFERSIZE];
    char currentdirectory[BUFFERSIZE];
    char * username = getenv("USER");

    gethostname(systemname, sizeof(systemname));
    getcwd(currentdirectory, sizeof(currentdirectory));
    if(strlen(currentdirectory) >= strlen(home))
    {
        printf("<%s@%s:~%s %s : %ld> ", username, systemname, currentdirectory + strlen(home), arg, seconds);
    }
    else
    {
        printf("<%s@%s:%s  %s : %ld> ", username, systemname, currentdirectory, arg, seconds);
    }

    char input[BUFFERSIZE];
    if (fgets(input, BUFFERSIZE, stdin) != NULL)
    {
        input[strcspn(input, "\n")] = '\0';
        processInput(input, home);
    }
}