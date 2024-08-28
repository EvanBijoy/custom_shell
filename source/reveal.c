#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "../headers/reveal.h"
#include "utils.h"

static char prev_dir[1024] = "";

void printFileInfo(const char *fileName, const struct stat *fileStat)
{
    printf( (S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf( (fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXOTH) ? "x " : "- ");

    printf("%ld ", fileStat->st_nlink);

    printf("%s %s ", getpwuid(fileStat->st_uid)->pw_name, getgrgid(fileStat->st_gid)->gr_name);

    printf("%5ld ", fileStat->st_size);

    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&fileStat->st_mtime));
    printf("%s ", timebuf);

    if(S_ISDIR(fileStat->st_mode))
    {
        printf("\033[1;33m%s\034[0m\n", fileName);
    }
    else if(fileStat->st_mode & S_IXUSR)
    {
        printf("\033[1;32m%s\033[0m\n", fileName);
    }
    else
    {
        printf("\033[0;37m%s\033[0m\n", fileName);
    }
}

void listFiles(const char *path, int showHidden, int showDetails)
{
    struct dirent *entry;
    struct stat fileStat;
    DIR *dp = opendir(path);

    if(dp == NULL)
    {
        perror("opendir");
        return;
    }

    while((entry = readdir(dp)) != NULL)
    {
        if(!showHidden && entry->d_name[0] == '.')
        {
            continue;
        }

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if(stat(fullPath, &fileStat) == -1)
        {
            perror("stat");
            continue;
        }

        if(showDetails)
        {
            printFileInfo(entry->d_name, &fileStat);
        }
        else
        {
            if(S_ISDIR(fileStat.st_mode))
            {
                printf("\033[1;34m%s\033[0m\n", entry->d_name);
            }
            else if(fileStat.st_mode & S_IXUSR)
            {
                printf("\033[1;32m%s\033[0m\n", entry->d_name);
            }
            else
            {
                printf("\033[0;37m%s\033[0m\n", entry->d_name);
            }
        }
    }

    closedir(dp);
}

void revealCommand(char *args[], char *home)
{
    char *new_path = NULL;
    char * dup;
    int showHidden = 0, showDetails = 0, start = 1;

    for(int i = 1; args[i] != NULL; i++)
    {
        if(args[i][0] == '-')
        {
            for(int j = 1; j < strlen(args[i]); j++)
            {
                if(args[i][j] == 'a')
                {
                    showHidden = 1;
                }
                if(args[i][j] == 'l')
                {
                    showDetails = 1;
                }
                if(args[i][j] != 'a' && args[i][j] != 'l')
                {
                    printf("INCORRECT FLAG\n");
                    return;
                }
            }
        }
        else
        {
            start = i;
        }
    }

    if(args[start] == NULL)
    {
        listFiles(home, showHidden, showDetails);
        return;
    }

    if(strcmp(args[start], "~") == 0)
    {
        new_path = home;
    }
    else if(strcmp(args[start], "-") == 0)
    {
        if(strcmp(prev_dir, "") == 0)
        {
            printf("ERROR: No previous directory to hop to.\n");
            return;
        }
        new_path = prev_dir;
    }
    else if(args[start][0] == '~')
    {
        dup = strdup(home);
        strcat(dup, args[start] + 1);
        new_path = dup;
    }
    else
    {
        new_path = args[start];
    }

    getcwd(prev_dir, sizeof(prev_dir)); 

    listFiles(new_path, showHidden, showDetails);
}
