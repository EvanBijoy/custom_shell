#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/log.h"

void procloreCommand(char *args[], char * home)
{
    char status, stat_path[256], exe_path[256];
    FILE *stat_file;
    unsigned long vm_size;
    pid_t pid;
    int pgrp;

    if(args[1] == NULL)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(args[1]);
    }

    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
    stat_file = fopen(stat_path, "r");
    if(stat_file == NULL)
    {
        printf("COULD NOT OPEN STAT FILE FOR THE PID: %d\n", pid);
        return;
    }

    fscanf(stat_file, "%*d %*s %c %*d %d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %lu", &status, &pgrp, &vm_size);
    fclose(stat_file);

    snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(exe_path, exe_path, sizeof(exe_path) - 1);
    if(len != -1)
    {
        exe_path[len] = '\0';
    }
    else
    {
        strcpy(exe_path, "UNABLE TO READ EXECUTABLE PATH");
    }

    printf("pid : %d\n", pid);
    printf("process status : %c%s\n", status, status == 'R' ? "+" : "");
    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %lu\n", vm_size);
    printf("executable path : %s\n", exe_path);
}
