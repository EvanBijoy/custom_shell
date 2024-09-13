#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../headers/activities.h"
#include "../headers/commands.h"

// Function to compare background processes by PID (for sorting by PID)
int compareByPID(const void *a, const void *b) 
{
    BackgroundProcess *procA = (BackgroundProcess *)a;
    BackgroundProcess *procB = (BackgroundProcess *)b;
    return procA->pid - procB->pid;
}

// Function to get the state of a process by checking its status
char *getProcessState(pid_t pid)
{
    char proc_path[256];
    char state;
    FILE *status_file;

    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
    status_file = fopen(proc_path, "r");

    if (status_file == NULL)
    {
        return "Terminated";
    }

    fscanf(status_file, "%*d %*s %c", &state);
    fclose(status_file);

    if (state == 'R')
        return "Running";
    else if (state == 'S')
        return "Stopped";
    else if (state == 'Z')
        return "Zombie";
    else
        return "Stopped";
}

void activitiesCommand()
{
    checkBackgroundProcesses();

    if (bg_count == 0)
    {
        printf("No background processes are currently running.\n");
        return;
    }

    // Sort the processes based on lexicographic order (by name)
    qsort(bg_processes, bg_count, sizeof(BackgroundProcess), compareByPID);

    // Print the list of background processes
    for (int i = 0; i < bg_count; i++)
    {
        char *state = getProcessState(bg_processes[i].pid);
        printf("%d : %s - %s\n", bg_processes[i].pid, bg_processes[i].name, state);
    }
}
