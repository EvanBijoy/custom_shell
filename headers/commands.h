#ifndef COMMANDS_H
#define COMMANDS_H

#include <sys/time.h>
#include <sys/types.h>

typedef struct BackgroundProcess 
{
    pid_t pid;
    char *name;
    struct timeval start_time;
} BackgroundProcess;

extern BackgroundProcess bg_processes[100];
extern int bg_count;

extern pid_t shell_pgid;
extern pid_t foreground_pid;

void executeCommand(char *command, char *home);
int countBackgroundProcesses();
void checkBackgroundProcesses();
void addBackgroundProcess(pid_t pid, const char *name);

#endif