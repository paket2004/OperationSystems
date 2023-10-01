#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

int main() {
    clock_t start_time_parent, end_time_parent;
    pid_t child1 = fork();
    start_time_parent = clock();
  if (child1 == 0) {
        // Child process 1 code
        clock_t start_time_child1, end_time_child1; 
        start_time_child1 = clock();  
        // we can add this for to see that code works, because without it answer is 0. for (int i = 0; i < 1000000; i++) {}     
        end_time_child1 = clock();
        double execution_time_child1 = (double)(end_time_child1 - start_time_parent) / CLOCKS_PER_SEC * 1000;
        printf("Child 1: PID = %d, PPID = %d, Execution Time = %.2lf ms\n", getpid(), getppid(), execution_time_child1);
        exit(0);
    }
    
    pid_t child2 = fork();
    if (child2 == 0) {
        // Child process 2 code
        clock_t start_time_child2, end_time_child2;    
        start_time_child2 = clock();
        // we can add this for to see that code works, because without it answer is 0. for (int i = 0; i < 1000000; i++) {} 
        end_time_child2 = clock();
        double execution_time_child2 = (double)(end_time_child2 - start_time_child2) / CLOCKS_PER_SEC * 1000;
        printf("Child 2: PID = %d, PPID = %d, Execution Time = %.2lf ms\n", getpid(), getppid(), execution_time_child2);
        exit(0);
    }
    
    end_time_parent = clock();
    double execution_time_parent = (double)(end_time_parent - start_time_parent) / CLOCKS_PER_SEC * 1000;
    printf("Parent: PID = %d, Execution Time = %.2lf ms\n", getpid(), execution_time_parent);
    
  
    wait(NULL);
    wait(NULL);
    
    return 0;
}
