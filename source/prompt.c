#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include "../headers/prompt.h"

#define BUFFERSIZE 4096

void showPrompt(char * home)
{
    char systemname[BUFFERSIZE];
    char currentdirectory[BUFFERSIZE];
    char * username = getenv("USER");

    gethostname(systemname, sizeof(systemname));
    getcwd(currentdirectory, sizeof(currentdirectory));
    if(strlen(currentdirectory) >= strlen(home))
    {
        printf("<%s@%s:~%s> ", username, systemname, currentdirectory + strlen(home));
    }
    else
    {
        printf("<%s@%s:%s> ", username, systemname, currentdirectory);
    }
}