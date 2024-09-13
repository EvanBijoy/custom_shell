#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <termios.h>
#include "../headers/neonate.h"

// Helper function to check if key 'x' is pressed
int isKeyPressed()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Turn off terminal echo and canonical mode
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Set the terminal to non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) 
    {
        ungetc(ch, stdin);
        return 1;  // Key pressed
    }

    return 0;  // No key pressed
}

// Function to get the most recent process ID from /proc
pid_t getMostRecentPID() 
{
    DIR *dir;
    struct dirent *entry;
    pid_t latest_pid = -1;

    dir = opendir("/proc");
    if (dir == NULL) 
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Iterate through the /proc directory
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR) 
        {
            char *end;
            pid_t pid = (pid_t) strtol(entry->d_name, &end, 10);
            if (*end == '\0') 
            {
                if (pid > latest_pid) 
                {
                    latest_pid = pid;
                }
            }
        }
    }

    closedir(dir);
    return latest_pid;
}

// Command handler for 'neonate'
void neonateCommand(char *args[])
{
    if (args[1] == NULL || strcmp(args[1], "-n") != 0 || args[2] == NULL) 
    {
        printf("Usage: neonate -n [time_arg]\n");
        return;
    }

    int time_interval = atoi(args[2]);
    if (time_interval <= 0) 
    {
        printf("Invalid time interval: %s\n", args[2]);
        return;
    }

    printf("Press 'x' to stop.\n");

    while (1) 
    {
        pid_t recent_pid = getMostRecentPID();
        if (recent_pid != -1) 
        {
            printf("%d\n", recent_pid);
        }

        sleep(time_interval);

        // Check if 'x' was pressed to terminate the loop
        if (isKeyPressed() && getchar() == 'x') 
        {
            break;
        }
    }
}
