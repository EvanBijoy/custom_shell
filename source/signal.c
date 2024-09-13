// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include "../headers/signal.h"
// #include "../headers/shared.h"

// pid_t foreground_pid = 0;
// int background_job_count = 0;

// void handle_sigint(int sig)
// {
//     if (foreground_pid != 0) 
//     {
//         kill(foreground_pid, SIGINT); // Send SIGINT only to the foreground process
//     }
//     printf("\n");
// }

// void handle_sigtstp(int sig) 
// {
//     if (foreground_pid != 0) 
//     {
//         kill(foreground_pid, SIGTSTP); // Send SIGTSTP only to the foreground process
//         printf("\n[%d] Stopped\n", foreground_pid);
//         // Move the process to the background
//         // Implement job control logic here (e.g., add to a list of background jobs)
//         background_job_count++;
//         foreground_pid = 0;
//     }
// }

// void setup_signal_handlers() 
// {
//     struct sigaction sa_int, sa_tstp;

//     // SIGINT (Ctrl+C)
//     sa_int.sa_handler = handle_sigint;
//     sigemptyset(&sa_int.sa_mask);
//     sa_int.sa_flags = SA_RESTART;
//     sigaction(SIGINT, &sa_int, NULL);

//     // SIGTSTP (Ctrl+Z)
//     sa_tstp.sa_handler = handle_sigtstp;
//     sigemptyset(&sa_tstp.sa_mask);
//     sa_tstp.sa_flags = SA_RESTART;
//     sigaction(SIGTSTP, &sa_tstp, NULL);
// }

// void cleanup_and_exit() 
// {
//     // Kill all background processes
//     // Implement logic to track and terminate all background processes
//     exit(0);
// }