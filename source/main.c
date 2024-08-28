#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/prompt.h"
#include "../headers/input.h"
#include "../headers/commands.h"

int main()
{
    char *input;
    int BUFFERSIZE = 2048;
    char home[BUFFERSIZE];
    
    input = (char *) malloc(BUFFERSIZE * sizeof(char));
    getcwd(home, sizeof(home));

    while(1)
    {
        showPrompt(home);
        scanf("%[^\n]", input);
        scanf("%*c");
        processInput(input, home);    
        checkBackgroundProcesses();
    }

    free(input);
    return 0;
}
