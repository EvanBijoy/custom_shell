#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "../headers/ping.h"

void pingCommand(char *args[])
{
    pid_t pid = atoi(args[1]);
    int signal_number = atoi(args[2]);

    // Take modulo 32 of the signal number
    signal_number %= 32;

    if (kill(pid, signal_number) == 0) 
    {
        printf("Sent signal %d to process with pid %d\n", signal_number, pid);
    } 
    else 
    {
        if (errno == ESRCH) 
        {
            printf("No such process found\n");
        } 
        else if (errno == EPERM) 
        {
            printf("Permission denied\n");
        } 
        else 
        {
            perror("Error sending signal");
        }
    }
}