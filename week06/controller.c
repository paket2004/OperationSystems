#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


pid_t agent_pid;

void handle_exit(int agent_pid) {
    // Send SIGUSR2 to the agent and exit
    if (kill(agent_pid, SIGUSR2) != 0) {
        perror("Failed to send SIGUSR2");
    }
    exit(0);
}

// Signal handler for SIGINT (Ctrl+C)
void sigint_handler(int signo) {
    if (signo == SIGINT) {
        // Send SIGTERM to the agent
        if (kill(agent_pid, SIGTERM) != 0) {
            perror("Failed to send SIGTERM");
        }
        printf("Terminating by SIGTERM (Ctrl+C) (for agent.c and then exit(0) for controller.c)\n");
    	exit(0);
    }
   
    
}


int main() {
    // Attempt to open the file.
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        perror("Error: No agent found.");
        return 1;
    }
	
    // If file is opened, attempt to read pid.
    if (fscanf(pid_file, "%d", &agent_pid) != 1) {
        perror("Failed to read agent's PID");
        fclose(pid_file);
        return 1;
    }

    fclose(pid_file);

    // File opened, pid taken, therefore agent found.
    printf("Agent found. Agent PID: %d\n", agent_pid);

	// Register the SIGINT signal handler
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Failed to register SIGINT handler");
        return 1;
    }
    // Infinite loop for user interaction
    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: \n");
        char command[20];
        if (scanf("%19s", command) != 1) {
            perror("Failed to read user command \n");
            return 1;
        }
	
        if (strcmp(command, "read") == 0) {
            // Send SIGUSR1 to the agent
            if (kill(agent_pid, SIGUSR1) != 0) {
                perror("Failed to send SIGUSR1");
            }
        } else if (strcmp(command, "exit") == 0) {
            handle_exit(agent_pid);
        } else if (strcmp(command, "stop") == 0) {
            // Send SIGSTOP to the agent
            if (kill(agent_pid, SIGSTOP) != 0) {
                perror("Failed to send SIGSTOP");
            }
        } else if (strcmp(command, "continue") == 0) {
            // Send SIGCONT to the agent
            if (kill(agent_pid, SIGCONT) != 0) {
                perror("Failed to send SIGCONT");
            }
        } else {
            printf("Invalid command. Please choose one of {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
        }
    }

    return 0;
}

