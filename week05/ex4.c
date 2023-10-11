#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

int c;
int k;
pthread_mutex_t lock_counter;
pthread_mutex_t lock_cur_number;


void* prime_counter(void * interval) {
    for(int j = 0; j < *(int*)interval; j++) {
        pthread_mutex_lock(&lock_cur_number);
            if(k <= 1) {
                k += 1;
                pthread_mutex_unlock(&lock_cur_number);
                continue;
            };
			if(k <= 3) {
                pthread_mutex_lock(&lock_counter);
                c++;
                k++;
                pthread_mutex_unlock(&lock_counter);
                pthread_mutex_unlock(&lock_cur_number);
                continue;
            }

            if(k % 2 == 0 || k % 3 == 0) {
                k++;
                pthread_mutex_unlock(&lock_cur_number);
                continue;
            };
			int check = 1;
            for(int i = 5; i * i <= k; i = i + 6) {
                if(k % i == 0 || k % (i + 2) == 0) {
                    check = 0;
                    break;
                }
            }
            if (check == 0) {
                k++;
                pthread_mutex_unlock(&lock_cur_number);
                continue;
            }

            pthread_mutex_lock(&lock_counter);
            k++;
            c++;
            pthread_mutex_unlock(&lock_counter);
        pthread_mutex_unlock(&lock_cur_number);
	}
}

int main(int argc, char *argv[]){
    if (argc < 2){
        exit(0);
    }
	c = 0;
    k = 0;
    pthread_mutex_init(&lock_counter, NULL); 
    pthread_mutex_init(&lock_cur_number, NULL); 
	int n, m;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    pthread_t threads[m];
    int part = (int)ceil(n/m);
    for (int i = 0; i < m; i++){
        int from = i * part;
        int to = part * (i+1) % n;
        if (to == 0) {
        	to = n;
		}
        int interval = to - from;
        pthread_create(&threads[i], NULL, prime_counter, &interval);   
    }
    
    for (int i = 0; i < m; i++){
        pthread_join(threads[i], NULL);
    }
    printf("%d\n", c);
    pthread_mutex_destroy(&lock_counter);
    pthread_mutex_destroy(&lock_cur_number);
}
