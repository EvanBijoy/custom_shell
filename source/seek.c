#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <unistd.h>
#include "../headers/log.h"

#define MAX_MATCHES 1000
#define BUFFERSIZE 4096

int directoryFlag = 0, fileFlag = 0, printFlag = 0, matchCount = 0;
char * target;
char matches[MAX_MATCHES][BUFFERSIZE];

int fileTreeWalk(const char *fpath, const struct stat *sb, int typeflag)
{
    if((typeflag == FTW_D && fileFlag) || (typeflag == FTW_F && directoryFlag))
    {
        return 0;
    }
    
    const char *filename = strrchr(fpath, '/');
    if(filename == NULL)
    {
        filename = fpath;
    }
    else
    {
        filename++;
    }

    if(strstr(filename, target) != NULL)
    {
        if(matchCount < MAX_MATCHES)
        {
            strcpy(matches[matchCount], fpath);
            matchCount++;
        }
    }
    return 0;
}


void seekCommand(char *args[], char * home)
{
    directoryFlag = 0, fileFlag = 0, printFlag = 0, matchCount = 0;
    target = NULL;
    char * search = ".";

    for(int i = 0; i < MAX_MATCHES; i++)
    {
        memset(matches[i], 0, sizeof(matches[i]));
    }

    for(int i = 1; args[i] != NULL; i++)
    {
        if(strcmp(args[i], "-d") == 0)
        {
            directoryFlag = 1;
        }
        else if(strcmp(args[i], "-f") == 0)
        {
            fileFlag = 1;
        }
        else if(strcmp(args[i], "-e") == 0)
        {
            printFlag = 1;
        }
        else if(target == NULL)
        {
            target = args[i];
        }
        else
        {
            search = args[i];
        }
    }

    if(fileFlag && directoryFlag)
    {
        printf("Invalid flags!");
    }

    ftw(search, fileTreeWalk, 20);

    if(matchCount == 0)
    {
        printf("No match found!\n");
    }
    else if(matchCount == 1 && printFlag)
    {
        struct stat st;
        stat(matches[0], &st);
        if(S_ISDIR(st.st_mode))
        {
            printf("\033[1;34m%s\033[0m\n", matches[0]);
            if(chdir(matches[0]) != 0)
            {
                printf("Missing permissions for task!\n");
            }
        }
        else if(S_ISREG(st.st_mode))
        {
            FILE *file = fopen(matches[0], "r");
            if(file)
            {
                char buffer[1024];
                while (fgets(buffer, sizeof(buffer), file) != NULL)
                {
                    printf("%s", buffer);
                }
                fclose(file);
            }
            else
            {
                printf("Missing permissions for task!\n");
            }
        }
    }
    else
    {
        for(int i = 0; i < matchCount; i++)
        {
            struct stat st;
            stat(matches[i], &st);
            if (S_ISDIR(st.st_mode))
            {
                printf("\033[1;34m%s\033[0m\n", matches[i]);
            }
            else
            {
                printf("\033[1;32m%s\033[0m\n", matches[i]);
            }
        }
    }
}
