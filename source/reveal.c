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
#include "../headers/shared.h"

#define BUFFERSIZE 4096

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
        printf("\033[1;34m%s\033[0m\n", fileName);
    }
    else if(fileStat->st_mode & S_IXUSR)
    {
        printf("\033[1;32m%s\033[0m\n", fileName);
    }
    else
    {
        printf("%s\n", fileName);
    }
}

void listFiles(const char *path, int showHidden, int showDetails)
{
    struct dirent *entry;
    struct stat fileStat;
    DIR *dp = opendir(path);

    if (dp == NULL)
    {
        perror("opendir");
        return;
    }

    // Store directory entries in an array
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort); // alphasort does the lexicographical sorting

    if (n < 0)
    {
        perror("scandir");
        closedir(dp);
        return;
    }

    // Iterate over sorted entries
    for (int i = 0; i < n; i++)
    {
        entry = namelist[i];

        if (!showHidden && entry->d_name[0] == '.')
        {
            free(namelist[i]);
            continue;
        }

        char fullPath[BUFFERSIZE];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &fileStat) == -1)
        {
            perror("stat");
            free(namelist[i]);
            continue;
        }

        if (showDetails)
        {
            printFileInfo(entry->d_name, &fileStat);
        }
        else
        {
            if (S_ISDIR(fileStat.st_mode))
            {
                printf("\033[1;34m%s\033[0m\n", entry->d_name);
            }
            else if (fileStat.st_mode & S_IXUSR)
            {
                printf("\033[1;32m%s\033[0m\n", entry->d_name);
            }
            else
            {
                printf("%s\n", entry->d_name);
            }
        }

        free(namelist[i]);
    }

    free(namelist);
    closedir(dp);
}


void revealCommand(char *args[], int count, char *home)
{
    char resolvedPath[BUFFERSIZE];
    char *new_path = NULL;
    char *dup;
    int showHidden = 0, showDetails = 0, start = 1, flag = 0;

    for(int i = 1; args[i] != NULL; i++)
    {
        if(args[i][0] == '-')
        {
            if(args[i][1] == '\0')
            {
                flag = 1;
            }
            else
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
        }
        else
        {
            start = i;
            break;
        }
    }

    if(start == count || (start == count - 1 && args[start][0] == '-' && strcmp(args[start], "-")))
    {
        new_path = getcwd(resolvedPath, BUFFERSIZE);
        if (new_path == NULL) {
            perror("getcwd");
            return;
        }
        listFiles(new_path, showHidden, showDetails);
        return;
    }

    if(strcmp(args[start], "~") == 0)
    {
        new_path = home;
    }
    else if(flag)
    {
        if(strcmp(prev_dir, "") == 0)
        {
            printf("ERROR: No previous directory to reveal.\n");
            return;
        }
        new_path = prev_dir;
    }
    else if(args[start][0] == '~')
    {
        new_path = (char*)malloc(strlen(home) + strlen(args[start]));
        if (new_path == NULL) {
            perror("malloc");
            return;
        }
        strcpy(new_path, home);
        strcat(new_path, args[start] + 1);
    }
    else
    {
        new_path = args[start];
    }

    listFiles(new_path, showHidden, showDetails);
}
