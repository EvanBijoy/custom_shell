#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/commands.h"
#include "../headers/input.h"
#include "../headers/shrc.h"
#include "../headers/piperedirection.h"

#define BUFFERSIZE 4096

void processInput(char *input, char *home)
{
    char *command;
    char *saveptr1;
    FILE *log_file;
    char log_file_path[BUFFERSIZE];

    // Prepare log file path
    snprintf(log_file_path, sizeof(log_file_path), "%s/command_log.txt", home);

    // Log commands unless it's a log command itself
    if (strstr(input, " log ") == NULL && strncmp(input, "log", 3) != 0)
    {
        log_file = fopen(log_file_path, "a");
        if (log_file == NULL)
        {
            perror("Failed to open log file for writing");
            return;
        }
        fprintf(log_file, "%s\n", input);
        fclose(log_file);
    }

    // Handle multiple commands separated by ';'
    command = strtok_r(input, ";", &saveptr1);
    while (command != NULL)
    {
        if (strchr(command, '|') != NULL || strchr(command, '>') != NULL || strchr(command, '<') != NULL)
        {
            executePipeRedirectionCommand(command, home);  // For commands with pipes
        }
        else
        {
            executeCommand(command, home);   // Regular commands
        }
        command = strtok_r(NULL, ";", &saveptr1);
    }
}
