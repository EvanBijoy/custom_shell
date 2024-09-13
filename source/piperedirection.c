#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../headers/piperedirection.h"
#include "../headers/commands.h"

#define MAX_COMMANDS 32
#define MAX_ARGUMENTS 10
#define BUFFERSIZE 4096

void executeSingleCommand(char *command, int input_fd, int output_fd, char *home) 
{
    int counter = 0;
    char *token, *saveptr;

    // Handle redirection within the command
    char *input_file = NULL;
    char *output_file = NULL;
    int append_mode = 0;

    char *input_redir = strstr(command, "<");
    char *output_redir = strstr(command, ">>");
    if (!output_redir) 
    {
        output_redir = strstr(command, ">");
    }

    if (input_redir) 
    {
        *input_redir = '\0';
        input_file = strtok(input_redir + 1, " \t\n");
    }

    if (output_redir) 
    {
        *output_redir = '\0';
        if (*output_redir == '>') 
        {
            append_mode = 1;
            output_file = strtok(output_redir + 2, " \t\n");
        } 
        else 
        {
            output_file = strtok(output_redir + 1, " \t\n");
        }
    }

    // Handle input redirection
    if (input_file) 
    {
        int fd = open(input_file, O_RDONLY);
        if (fd < 0) 
        {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    } 
    else if (input_fd != STDIN_FILENO) 
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    // Handle output redirection
    if (output_file)
    {
        int flags = O_WRONLY | O_CREAT;
        if (append_mode) 
        {
            flags |= O_APPEND;
        } 
        else 
        {
            flags |= O_TRUNC;
        }
        int fd = open(output_file, flags, 0644);
        if (fd < 0) 
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    } 
    else if (output_fd != STDOUT_FILENO) 
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    executeCommand(command, home);
}

void executePipeRedirectionCommand(char *command, char *home)
{
    char *commands[MAX_COMMANDS];
    int num_pipes = 0;
    char *token, *saveptr;
    int pipefds[2 * (MAX_COMMANDS - 1)];

    token = strtok_r(command, "|", &saveptr);
    if(token == NULL)
    {
        executeSingleCommand(commands, STDIN_FILENO, STDOUT_FILENO, home);
    }
    while (token != NULL && num_pipes < MAX_COMMANDS) 
    {
        commands[num_pipes++] = token;
        token = strtok_r(NULL, "|", &saveptr);
    }

    // Create pipes for intermediate commands
    for (int i = 0; i < num_pipes - 1; i++) 
    {
        if (pipe(pipefds + i * 2) < 0) 
        {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // Execute each command
    for (int i = 0; i < num_pipes; i++) 
    {
        pid_t pid = fork();
        if (pid == 0) 
        {
            // Set up input
            int input_fd = (i == 0) ? STDIN_FILENO : pipefds[(i - 1) * 2];

            // Set up output
            int output_fd = (i == num_pipes - 1) ? STDOUT_FILENO : pipefds[i * 2 + 1];

            // Close all unnecessary pipe ends
            for (int j = 0; j < 2 * (num_pipes - 1); j++) 
            {
                if (pipefds[j] != input_fd && pipefds[j] != output_fd) 
                {
                    close(pipefds[j]);
                }
            }

            // Execute the command
            executeSingleCommand(commands[i], input_fd, output_fd, home);
            exit(EXIT_FAILURE); // Should not reach here
        } 
        else if (pid < 0) 
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process closes all pipe ends
    for (int i = 0; i < 2 * (num_pipes - 1); i++) 
    {
        close(pipefds[i]);
    }

    // Wait for all child processes to complete
    for (int i = 0; i < num_pipes; i++) 
    {
        wait(NULL);
    }
}