#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

// Signal handler for SIGUSR1
void sigusr1_handler(int signo) {
    if (signo == SIGUSR1) {
        // Read and print the contents of text.txt
        int text_file = open("text.txt", O_RDONLY);
        if (text_file == -1) {
            perror("Failed to open text.txt");
        } else {
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(text_file, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, (size_t)bytes_read);
            }
            close(text_file);
        }
    }
}


// Signal handler for SIGUSR2
void sigusr2_handler(int signo) {
    if (signo == SIGUSR2) {
      printf("Process terminating...\n");
      exit(0);
    }
}


int main() {
    // Create a PID file in /var/run/agent.pid
    int pid_file = open("/var/run/agent.pid", O_WRONLY | O_CREAT, 0644);
    if (pid_file == -1) {
        perror("Failed to create PID file");
        return 1;
    }

    // Write the PID of the agent process to the PID file
    dprintf(pid_file, "%d\n", getpid());

    // Close the PID file
    close(pid_file);

    // Register the SIGUSR1 signal handler
    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        perror("Failed to register SIGUSR1 handler");
        return 1;
    }
    
    // Register the SIGUSR2 signal handler
    if (signal(SIGUSR2, sigusr2_handler) == SIG_ERR) {
        perror("Failed to register SIGUSR2 handler");
        return 1;
    }

    // Sleep indefinitely
    while (1) {
        pause();
    }

    return 0;
}
