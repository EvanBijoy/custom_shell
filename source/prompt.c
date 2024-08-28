#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include "../headers/prompt.h"

void showPrompt(char * home)
{
    int BUFFERSIZE = 2048;
    char systemname[BUFFERSIZE];
    char currentdirectory[BUFFERSIZE];
    char * username = getenv("USER");

    gethostname(systemname, sizeof(systemname));
    getcwd(currentdirectory, sizeof(currentdirectory));

    printf("<%s@%s:~%s> ", username, systemname, currentdirectory + strlen(home));
}