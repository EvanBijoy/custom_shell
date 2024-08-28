#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include "../headers/commands.h"
#include "../headers/hop.h"
#include "../headers/reveal.h"
#include "../headers/log.h"
#include "../headers/seek.h"
#include "../headers/proclore.h"

void executeCommand(char *command, char *home)
{
    char *args[10];
    int counter = 0;
    int bgdflag = 0;
    pid_t pid;
    char *token;
    char *saveptr2;

    char *bg_cmd = strstr(command, "&");
    if(bg_cmd != NULL)
    {
        *bg_cmd = '\0';
        bgdflag = 1;
    }

    token = strtok_r(command, " \t", &saveptr2);
    while(token != NULL && counter < 10)
    {
        args[counter++] = token;
        token = strtok_r(NULL, " \t", &saveptr2);
    }

    if(counter == 0)
    {
        return;
    }

    args[counter] = NULL;

    // HOP COMMAND
    if(strcmp(args[0], "hop") == 0)
    {
        if(bgdflag == 1)
        {
            pid = fork();
            if(pid == 0)
            {
                hopCommand(args, home);
                exit(0);
            }
            else
            {
                printf("Background PID: %d\n", pid);
            }
        }
        else
        {
            hopCommand(args, home);
        }
        return;
    }

    // REVEAL COMMAND
    if(strcmp(args[0], "reveal") == 0)
    {
        if(bgdflag == 1)
        {
            pid = fork();
            if(pid == 0)
            {
                revealCommand(args, home);
                exit(0);
            }
            else
            {
                printf("Background PID: %d\n", pid);
            }
        }
        else
        {
            revealCommand(args, home);
        }
        return;
    }

    // LOG COMMAND
    if(strcmp(args[0], "log") == 0)
    {
        if(bgdflag == 1)
        {
            pid = fork();
            if(pid == 0)
            {
                logCommand(args, home);
                exit(0);
            }
            else
            {
                printf("Background PID: %d\n", pid);
            }
        }
        else
        {
            logCommand(args, home);
        }
        return;
    }

    // SEEK COMMAND
    if(strcmp(args[0], "seek") == 0)
    {
        if(bgdflag == 1)
        {
            pid = fork();
            if(pid == 0)
            {
                seekCommand(args, home);
                exit(0);
            }
            else
            {
                printf("Background PID: %d\n", pid);
            }
        }
        else
        {
            seekCommand(args, home);
        }
        return;
    }

    // PROCLORE COMMAND
    if(strcmp(args[0], "proclore") == 0)
    {
        if(bgdflag == 1)
        {
            pid = fork();
            if(pid == 0)
            {
                procloreCommand(args, home);
                exit(0);
            }
            else
            {
                printf("Background PID: %d\n", pid);
            }
        }
        else
        {
            procloreCommand(args, home);
        }
        return;
    }

    // DEFAULT COMMANDS    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    pid = fork();
    if(pid == 0)
    {
        if(execvp(args[0], args) == -1)
        {
            printf("ERROR: '%s' is not a valid command\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if(pid != 0)
    {
        if(bgdflag == 0)
        {
            wait(NULL);
            gettimeofday(&end, NULL);

            long seconds = (end.tv_sec - start.tv_sec);

            if (seconds > 2) {
                printf("Process %s took %lds to complete.\n", args[0], seconds);
            }
        }
        else
        {
            printf("Background PID: %d\n", pid);
        }
    }

    if (bg_cmd != NULL && *(bg_cmd + 1) != '\0')
    {
        executeCommand(bg_cmd + 1, home);
    }
}

void checkBackgroundProcesses()
{
    int status;
    pid_t pid;
    
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if(WIFEXITED(status))
        {
            printf("Process with PID %d ended normally.\n", pid);
        }
        else if(WIFSIGNALED(status))
        {
            printf("Process with PID %d ended abnormally.\n", pid);
        }
    }
}
