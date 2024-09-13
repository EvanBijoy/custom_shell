#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/log.h"
#include "../headers/input.h"

#define BUFFERSIZE 4096
#define LOG_FILE "command_log.txt"

void logCommand(char *args[], char *home)
{
    FILE *log_file;

    char log_file_path[BUFFERSIZE];
    snprintf(log_file_path, sizeof(log_file_path), "%s/command_log.txt", home);
    
    if(args[1] == NULL)
    {
        char command[BUFFERSIZE];
        int currIndex = 1, index = 1, lineCount = 0;

        log_file = fopen(log_file_path, "r");
        if (log_file == NULL)
        {
            perror("Failed to open log file");
            return;
        }

        while(fgets(command, sizeof(command), log_file) != NULL)
        {
            lineCount++;
        }
        rewind(log_file);
        
        while (fgets(command, sizeof(command), log_file) != NULL)
        {
            currIndex++;
            if(currIndex > lineCount - 14)
            {
                printf("%d %s", index++, command);
            }
        }
        fclose(log_file);
    }
    else if(strcmp(args[1], "purge") == 0)
    {
        log_file = fopen(log_file_path, "w");
        if (log_file == NULL)
        {
            perror("Failed to purge log file");
            return;
        }
        fclose(log_file);
    }
    else if(strcmp(args[1], "execute") == 0)
    {
        int lineCount = 0;

        if(args[2] == NULL)
        {
            printf("INVALID FORMAT\n");
            return;
        }

        int cmd_index = atoi(args[2]);
        char command[BUFFERSIZE];
        int current_index = 1;

        log_file = fopen(log_file_path, "r");
        if(log_file == NULL)
        {
            perror("Failed to open log file");
            return;
        }

        while(fgets(command, sizeof(command), log_file) != NULL)
        {
            lineCount++;
        }
        rewind(log_file);

        int target_index = lineCount - cmd_index + 1;

        if(target_index <= 0)
        {
            printf("ERROR: Command index out of range.\n");
            fclose(log_file);
            return;
        }

        // Iterate through the file to find and execute the desired command
        while(fgets(command, sizeof(command), log_file) != NULL)
        {
            if(current_index == target_index)
            {
                strtok(command, "\n"); // Remove the newline character
                processInput(command, home); // Execute the command
                break;
            }
            current_index++;
        }

        fclose(log_file);
    }
    else
    {
        printf("INCORRECT FLAG\n");
    }
}
