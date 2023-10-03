#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


int dotProduct(int* u, int* v, int start, int end) {
    long long result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}

int main() {
    int n;
    long long result = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int u[120];
    int v[120];
    int numOfProcesses = 1;
	
    for (int i = 0; i < 120; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    pid_t child_pids[n];

    // Create a file for reading and writing computation results
    FILE* file = fopen("temp.txt", "w+");
	pid_t dot_product = fork();
	if (dot_product > 0) {
		numOfProcesses++;
	}
	while(numOfProcesses < n && dot_product > 0) {
		dot_product = fork();
		if (dot_product > 0) {
			numOfProcesses++;
		}
	}
	long long res = 0;
	if (dot_product >= 0) {
		int start = (numOfProcesses - 1) * (120 / n);
		int end = (numOfProcesses) * (120 / n);
		res = dotProduct(u,v,start,end);
		fprintf(file,"%lld\n", res);
	}		
	
	int status = 0;
	while (wait(&status) > 0);
		if (dot_product > 0) {
		fclose(file); // Close the file
		FILE* file2 = fopen("temp.txt", "r");
		while(!feof(file2)) {
			char buf[10];
			fscanf(file2, "%s[^\n]", buf);
	   	   	result += atoi(buf); 
		}
    	printf("Total Dot Product: %lld\n", result);
    }
    

    return 0;
}
