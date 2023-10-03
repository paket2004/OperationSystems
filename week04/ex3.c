#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
int power(int num1, int num2) {
	int res = 1;
	for (int i = 0; i < num2; i++) {
		res *= num1;
	}
	return res;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    pid_t child;
  	int num_of_processes = 1;
    for (int i = 0; i < n; i++) {
        child = fork();
        sleep(5);
      }
	// Wait for all child processes to finish
	
    for (int i = 0; i < power(2,n); i++) {
        wait(NULL);
    }
    
    

    return 0;
}
