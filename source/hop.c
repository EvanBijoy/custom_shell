#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/hop.h"
#include "../headers/shared.h"

#define BUFFERSIZE 4096

void hopCommand(char *args[], char *home)
{
    char cwd[BUFFERSIZE];
    char *new_path = NULL;

    if(args[1] == NULL)
    {
        if(chdir(home) == -1)
        {
            perror("ERROR CHANGING DIRECTORIES");
            return;
        }

        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return;
    }

    for(int i = 1; args[i] != NULL; i++)
    {
        if(strcmp(args[i], "~") == 0)
        {
            new_path = home;
        }
        else if(strcmp(args[i], "-") == 0)
        {
            if(strcmp(prev_dir, "") == 0)
            {
                printf("ERROR: OLDPWD not set\n");
                return;
            }
            new_path = prev_dir;
        }
        else if(args[i][0] == '~')
        {
            new_path = strdup(home);
            if(new_path == NULL)
            {
                perror("ERROR ALLOCATING MEMORY");
                return;
            }
            strcat(new_path, args[i] + 1);
        }
        else
        {
            new_path = args[i];
        }

        getcwd(cwd, sizeof(cwd));

        if(chdir(new_path) == -1)
        {
            perror("ERROR CHANGING DIRECTORIES");
            return;
        }
        
        strcpy(prev_dir, cwd);
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
}
