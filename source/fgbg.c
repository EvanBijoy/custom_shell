#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>
#include "../headers/fgbg.h"

// fg <pid> implementation
void fgCommand(char *args[]) 
{
    if (args[1] == NULL) 
    {
        printf("fg: missing argument <pid>\n");
        return;
    }
    
    pid_t pid = atoi(args[1]);

    // Check if the process exists
    if (kill(pid, 0) == -1) 
    {
        if (errno == ESRCH) 
        {
            printf("No such process found\n");
        } 
        else 
        {
            perror("fg");
        }
        return;
    }

    // Resume the process if it was stopped
    if (kill(pid, SIGCONT) == -1) 
    {
        perror("fg: could not send SIGCONT");
        return;
    }

    // Bring the process to the foreground
    tcsetpgrp(STDIN_FILENO, getpgid(pid));

    // Wait for the process to finish or stop
    int status;
    while (waitpid(pid, &status, WUNTRACED) == -1 && errno == EINTR)
    {
        // Restart waitpid if interrupted by a signal
    }

    // Restore control to the shell process group
    tcsetpgrp(STDIN_FILENO, getpgid(getpid()));

    // If the process was stopped, notify the user
    if (WIFSTOPPED(status)) 
    {
        printf("[%d] stopped\n", pid);
    }
    else if (WIFEXITED(status)) 
    {
        printf("Process [%d] exited with status %d\n", pid, WEXITSTATUS(status));
    } 
    else if (WIFSIGNALED(status)) 
    {
        printf("Process [%d] killed by signal %d\n", pid, WTERMSIG(status));
    }
}


// bg <pid> implementation
void bgCommand(char *args[]) 
{
    if (args[1] == NULL) 
    {
        printf("bg: missing argument <pid>\n");
        return;
    }

    pid_t pid = atoi(args[1]);

    // Check if the process exists
    if (kill(pid, 0) == -1) 
    {
        if (errno == ESRCH) 
        {
            printf("No such process found\n");
        } 
        else 
        {
            perror("bg");
        }
        return;
    }

    // Resume the process in the background
    if (kill(pid, SIGCONT) == -1) 
    {
        perror("bg");
        return;
    }

    printf("[%d] continued\n", pid);
}
