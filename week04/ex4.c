#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 100
#define MAX_ARG_COUNT 10

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("Shell: ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove newline character
        input[strlen(input) - 1] = '\0';

        // Exit if the user enters "exit"
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Determine if the command should run in the background
        // % for the background
        int run_in_background = 0;
        if (input[strlen(input) - 1] == '%') {
            run_in_background = 1;
            input[strlen(input) - 1] = '\0'; // Remove the '&' character
        }

        // Fork a child process
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            char *args[MAX_ARG_COUNT];
            int arg_count = 0;

            // Tokenize the input
            char *token = strtok(input, " ");
            while (token != NULL && arg_count < MAX_ARG_COUNT - 1) {
                args[arg_count] = token;
                token = strtok(NULL, " ");
                arg_count++;
            }
            args[arg_count] = NULL; // Null-terminate the argument list

            // Execute the command
            execvp(args[0], args);

            // If execvp fails, print an error message
            perror("Command execution failed");
            exit(1);
        } else {
            // Parent process
            if (!run_in_background) {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    printf("Goodbye!\n");
    return 0;
}
