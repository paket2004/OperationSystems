#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>
#include<math.h>

void* prime_counter(void *range){
    int from = ((int*)range)[0];
    int to = ((int*)range)[1];
    int *counter = malloc(sizeof(int));
    for (int j = from;j < to;j++){
    	
        if (j <= 1) continue;
        if (j <= 3){
            *counter += 1;
            continue;
        }

        bool flag = true;
        for (int i = 2;i * i <= j;i++){
           if (j % i == 0) {
            flag = false;
            break;
           }
        }
        
        if (flag == true) {
            *counter += 1; 
        }
    }
    return (void*)counter;
}


int main(int argc, char* argv[]){
    if (argc < 2){
        exit(0);
    }
    int n, m;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    int ans = 0;
    pthread_t threads[m];
    
    int part = ceil(n/m);
    for (int i = 0;i < m;i++){
        int from = i * part;
        int to = (part * (i+1)) % n;
        if (to == 0) to = n;

        int *interval = (int*)calloc(2, sizeof(int));
        interval[0] = from;
        interval[1] = to;
        pthread_create(&(threads[i]), NULL, prime_counter, interval);        
    }
    
    for (int i = 0;i < m;i++){
    	void * local_amount = NULL;
        pthread_join(threads[i], &local_amount);
        ans += *(int*)local_amount;
    }
    
    printf("%d\n", ans);
}
