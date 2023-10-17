#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
#include <stdbool.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct{
	int idx, num; // process idx (index)
	int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
	int burst; // remaining burst (this should decrement when the process is being executed)
	bool stop,done;
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

//quantums
int quant, cur_quant;

void read_file(FILE* file){

    // extract the data of processes from the {file} 
    // and store them in the array {data}
    // initialize ps array to zeros (the process is terminated or not created yet)
    for (int i = 0; i < PS_MAX; i++) {
    	ps[i] = 0;
    }
	char buf[64];
	int count = -1;
	char* token;
	while (fgets(buf, 64, file) != NULL) {
		if (count != -1) {
			token = strtok(buf, " ");
			int tmp[3];
			int k = 0;
			while (token != NULL) {
				tmp[k++] = atoi(token);
				token = strtok(NULL, " ");
			}
			data[count].idx = tmp[0];
			data[count].at = tmp[1];
			data[count].bt = tmp[2];
			data[count].burst = tmp[2];
		}
		count++;
	}
	count--;
	data_size = count + 1;
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    //  send signal SIGCONT to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (process > 0) kill(process, SIGCONT);
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    //  send signal SIGTSTP to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (process > 0) kill(process, SIGTSTP);
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    //  send signal SIGTERM to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (process > 0) kill (process, SIGTERM);
}

// create a process using fork
void create_process(int new_process) {

    //  stop the running process
    if (ps[running_process] > 0) suspend(ps[running_process]);

    //  fork a new process and add it to ps array
	pid_t pid = fork();
	if (pid == 0) {
		char ind[10];
		sprintf(ind, "%d", new_process);
		char* argv[] = {"./worker",ind,NULL};
		execvp(argv[0], argv);
	}
	ps[new_process] = pid;
	running_process = new_process;
}

// find next process for running
ProcessData find_next_process() {
    int location = -1;
    int min_burst = INT_MAX;

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && data[i].burst < min_burst) {
            min_burst = data[i].burst;
            location = i;
        }
    }

    if (location == -1) {
        // No process to run, return an empty process
        ProcessData empty_process = { .idx = -1 };
        return empty_process;
    }

    if (data[location].at > total_time) {
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        total_time++;
        return find_next_process();
    }

    return data[location];
}

// reports the metrics and simulation results
void report(){
	printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].at);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

	for(int i = 0; i < data_size; i++)
		if (data[i].burst > 0)
		    return;

    // report simulation results
    report();

    // terminate the scheduler :)
	  exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    total_time++;

    if (running_process != -1) {
        cur_quant--;

        if (cur_quant == 0 || data[running_process].burst == 0) {
            // If the time quantum expires or the process finishes its burst, suspend it
            data[running_process].stop = true;
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
            cur_quant = quant;
        } else {
            // Decrement the remaining burst time
            data[running_process].burst--;
            printf("Scheduler: Runtime: %u seconds\n", total_time);
            printf("Scheduler: Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

            if (data[running_process].burst == 0) {
                cur_quant = quant;
                printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
                data[running_process].done = true;
                terminate(ps[running_process]);
                data[running_process].ct = total_time;
                data[running_process].tat = total_time - data[running_process].at;
                data[running_process].wt = data[running_process].tat - data[running_process].bt;
                ps[running_process] = -1;
                running_process = -1;
            }
        }
    }

    check_burst();
    ProcessData next_process = find_next_process();

    if (next_process.idx != running_process) {
        running_process = next_process.idx;

        if (data[running_process].stop) {
            data[running_process].num = total_time;
            cur_quant = quant;
            data[running_process].stop = false;
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            resume(ps[running_process]);
        } else {
            data[running_process].num = total_time;
            cur_quant = quant;
            printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            data[running_process].rt = total_time - data[running_process].at;
            create_process(data[running_process].idx);
        }
    }
}

int main(int argc, char *argv[]) {

    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
  	if (in_file == NULL) {   
		printf("File is not found or cannot open it!\n"); 
  		exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1); // infinite loop
}
