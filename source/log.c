#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/log.h"

void logCommand(char *args[], char *home)
{
    // Sample implementation: prints a message to the console
    if (args[1] != NULL) {
        printf("Logging message: %s\n", args[1]);
    } else {
        printf("Log command requires a message argument.\n");
    }
}
