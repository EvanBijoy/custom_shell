    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include "../headers/hop.h"
    #include "utils.h"

    static char prev_dir[1024] = "";

    void hopCommand(char *args[], char *home)
    {
        char cwd[1024];
        char *new_path = NULL;
        char * dup;

        if(args[1] == NULL)
        {
            if(chdir(home) == -1)
            {
                perror("ERROR CHANGING DIRECTORIES");
                return;
            }

            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
            return;
        }

        for (int i = 1; args[i] != NULL; i++)
        {
            if(strcmp(args[i], "~") == 0)
            {
                new_path = home;
            }
            else if(strcmp(args[i], "-") == 0)
            {
                if(strcmp(prev_dir, "") == 0)
                {
                    printf("ERROR: No previous directory to hop to.\n");
                    return;
                }
                new_path = prev_dir;
            }
            else if(args[i][0] == '~')
            {
                dup = strdup(home);
                strcat(dup, args[i] + 1);
                new_path = dup;
            }
            else
            {
                new_path = args[i];
            }

            getcwd(prev_dir, sizeof(prev_dir)); 

            if(chdir(new_path) == -1)
            {
                perror("ERROR CHANGING DIRECTORIES");
                return;
            }
    
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
    }
