#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include "../headers/prompt.h"
#include "../headers/input.h"
#include "../headers/commands.h"
#include "../headers/shared.h"
#include "../headers/shrc.h"

#define BUFFERSIZE 4096

char *input;
char home[BUFFERSIZE];
char prev_dir[BUFFERSIZE] = "";
pid_t shell_pgid;

void handle_sigint(int sig)
{
    if (foreground_pid > 0)
    {
        kill(-foreground_pid, SIGINT);
    }
    else
    {
        printf("\n");
        showPrompt(home);
        fflush(stdout);
    }
}

void handle_sigtstp(int sig)
{
    if (foreground_pid > 0)
    {
        kill(-foreground_pid, SIGTSTP);
    }
}

void setup_signal_handlers()
{
    struct sigaction sa_int, sa_tstp;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    sa_tstp.sa_handler = handle_sigtstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL);
}

int main()
{
    input = (char *) malloc(BUFFERSIZE * sizeof(char));
    getcwd(home, sizeof(home));
    readMyshrc();

    shell_pgid = getpid();
    if (setpgid(shell_pgid, shell_pgid) < 0)
        perror("Couldn't put the shell in its own process group");
    
    // Ignore interactive and job-control signals
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    if (tcsetpgrp(STDIN_FILENO, shell_pgid) < 0)
        perror("Couldn't grab control of terminal");

    setup_signal_handlers();

    while (1)
    {
        showPrompt(home);
        fflush(stdout);

        if (fgets(input, BUFFERSIZE, stdin) == NULL)
        {
            if (feof(stdin))
            {
                printf("\n");
                break; // Handle Ctrl+D (EOF), exit the shell
            }
            continue; // Handle other read errors
        }
        input[strcspn(input, "\n")] = 0; // Remove newline character

        if (strlen(input) > 0)
        {
            processInput(input, home);
        }
        fflush(stdout);

        checkBackgroundProcesses();
    }

    free(input);
    return 0;
}