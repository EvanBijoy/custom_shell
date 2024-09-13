#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include "../headers/commands.h"
#include "../headers/hop.h"
#include "../headers/reveal.h"
#include "../headers/log.h"
#include "../headers/seek.h"
#include "../headers/proclore.h"
#include "../headers/specialprompt.h"
#include "../headers/activities.h"
#include "../headers/ping.h"
#include "../headers/fgbg.h"

BackgroundProcess bg_processes[100];
int bg_count = 0;
pid_t foreground_pid;


void addBackgroundProcess(pid_t pid, const char *name) 
{
    if (bg_count < 100) 
    {
        bg_processes[bg_count].pid = pid;
        bg_processes[bg_count].name = strdup(name);
        gettimeofday(&bg_processes[bg_count].start_time, NULL);
        bg_count++;
    } 
    else 
    {
        fprintf(stderr, "Maximum number of background processes reached\n");
    }
}

int countBackgroundProcesses()
{
    return bg_count;
}

void executeCommand(char *command, char *home)
{
    char *args[10];
    int counter = 0;
    int bgdflag = 0;
    pid_t pid;
    char *token;
    char *saveptr2;
    char *interactivePrograms[] = {"vim", "nano", "less", "more", NULL};

    char *bg_cmd = strstr(command, "&");
    if (bg_cmd != NULL)
    {
        *bg_cmd = '\0';
        bgdflag = 1;
    }

    token = strtok_r(command, " \t", &saveptr2);
    while (token != NULL && counter < 10)
    {
        args[counter++] = token;
        token = strtok_r(NULL, " \t", &saveptr2);
    }

    if (counter == 0)
    {
        return;
    }

    args[counter] = NULL;

    // HOP COMMAND
    if (strcmp(args[0], "hop") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                hopCommand(args, home);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            hopCommand(args, home);
        }
        return;
    }

    // REVEAL COMMAND
    if (strcmp(args[0], "reveal") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                revealCommand(args, counter, home);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            revealCommand(args, counter, home);
        }
        return;
    }

    // LOG COMMAND
    if (strcmp(args[0], "log") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                logCommand(args, home);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            logCommand(args, home);
        }
        return;
    }

    // SEEK COMMAND
    if (strcmp(args[0], "seek") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                seekCommand(args, home);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            seekCommand(args, home);
        }
        return;
    }

    // PROCLORE COMMAND
    if (strcmp(args[0], "proclore") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                procloreCommand(args, home);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            procloreCommand(args, home);
        }
        return;
    }

    // ACTIVITIES COMMAND
    if (strcmp(args[0], "activities") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                activitiesCommand();
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            activitiesCommand();
        }
        return;
    }

    // PING COMMAND
    if (strcmp(args[0], "ping") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                pingCommand(args);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            pingCommand(args);
        }
        return;
    }

    // FG COMMAND
    if (strcmp(args[0], "fg") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                fgCommand(args);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            fgCommand(args);
        }
        return;
    }

    // BG COMMAND
    if (strcmp(args[0], "bg") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                bgCommand(args);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            bgCommand(args);
        }
        return;
    }

    // NEONATE COMMAND
    struct timeval st, en;
    gettimeofday(&st, NULL);

    if (strcmp(args[0], "neonate") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                neonateCommand(args);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            neonateCommand(args);
            gettimeofday(&en, NULL);

            long seconds = (en.tv_sec - st.tv_sec);

            if (seconds > 2)
            {
                showSpecialPrompt(home, seconds, args[0]);
            }
        }
        return;
    }

    // IMAN COMMAND
    if (strcmp(args[0], "iMan") == 0)
    {
        if (bgdflag == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                iManCommand(args);
                exit(0);
            }
            else
            {
                bg_processes[bg_count].pid = pid;
                bg_processes[bg_count].name = strdup(args[0]);
                gettimeofday(&bg_processes[bg_count].start_time, NULL);
                bg_count++;
                printf("[%d] %d\n", bg_count, pid);
            }
        }
        else
        {
            iManCommand(args);
        }
        return;
    }

    // DEFAULT COMMANDS    
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid = fork();
    if (pid == 0)
    {
        pid_t child_pid = getpid();
        setpgid(child_pid, child_pid);
        if (bgdflag)
        {
            int isInteractive = 0;
            for (int i = 0; interactivePrograms[i] != NULL; i++)
            {
                if (strcmp(args[0], interactivePrograms[i]) == 0)
                {
                    isInteractive = 1;
                    break;
                }
            }
            if(isInteractive)
            {
                int dev_null = open("/dev/null", O_WRONLY);
                dup2(dev_null, STDIN_FILENO);
                dup2(dev_null, STDOUT_FILENO);
                dup2(dev_null, STDERR_FILENO);
                close(dev_null);
            }
        }

        if (execvp(args[0], args) == -1)
        {
            printf("ERROR: '%s' is not a valid command\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
    {
        setpgid(pid, pid);

        if (bgdflag == 0)
        {
            foreground_pid = pid;
            tcsetpgrp(STDIN_FILENO, pid);

            int status;
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, shell_pgid);
            foreground_pid = 0;

            if (WIFSTOPPED(status))
            {
                printf("\n[%d] Stopped                 %s\n", bg_count + 1, args[0]);
                addBackgroundProcess(pid, args[0]);
            }
            else if (WIFSIGNALED(status))
            {
                printf("\n[%d] Terminated              %s\n", bg_count + 1, args[0]);
            }

            gettimeofday(&end, NULL);

            long seconds = (end.tv_sec - start.tv_sec);

            if (seconds > 2)
            {
                showSpecialPrompt(home, seconds, args[0]);
            }
        }
        else
        {
            addBackgroundProcess(pid, args[0]);
            printf("[%d] %d\n", bg_count, pid);
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
    char *name;
    struct timeval end_time;

    for (int i = 0; i < bg_count; i++)
    {
        pid = waitpid(bg_processes[i].pid, &status, WNOHANG);
        if (pid > 0)
        {
            name = bg_processes[i].name;
            gettimeofday(&end_time, NULL);
            
            if (WIFEXITED(status))
            {
                printf("%s with PID %d exited normally\n", name, pid);
            }
            // else if (WIFSIGNALED(status))
            // {
            //     printf("%s with PID %d exited abnormally\n", name, pid);
            // }
            
            // Remove the process from the bg_processes list
            for (int j = i; j < bg_count - 1; j++)
            {
                bg_processes[j] = bg_processes[j + 1];
            }
            bg_count--;
            i--; // Adjust index to account for removed process
        }
        else if (pid == 0)
        {
            // Process is still running, do nothing
        }
        else
        {
            // Error occurred, remove the process from the list
            printf("Error checking status of %s with PID %d\n", bg_processes[i].name, bg_processes[i].pid);
            for (int j = i; j < bg_count - 1; j++)
            {
                bg_processes[j] = bg_processes[j + 1];
            }
            bg_count--;
            i--; // Adjust index to account for removed process
        }
    }
}