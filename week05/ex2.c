#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_MESSAGE_SIZE 256

// Define a struct to hold thread information
struct Thread {
    pthread_t id;
    int i;
    char message[MAX_MESSAGE_SIZE];
};

// Function that each thread will run
void* thread_routine(void* arg) {
    struct Thread* thread_data = (struct Thread*)arg;

    // Access the thread-specific fields
    pthread_t thread_id = thread_data->id;
    int i = thread_data->i;
    char* message = thread_data->message;

    printf("Thread %d (ID: %lu) says: %s\n", i, (unsigned long)thread_id, message);

    return NULL;
}

int main() {
    int n = 10; // Number of threads
    struct Thread threads[n];

    // Create and initialize the thread data
    for (int i = 0; i < n; i++) {
        threads[i].i = i + 1;
        snprintf(threads[i].message, sizeof(threads[i].message), "Hello from Thread %d!", threads[i].i);

        // Create the thread
        if (pthread_create(&threads[i].id, NULL, thread_routine, &threads[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        if (pthread_join(threads[i].id, NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    

    return 0;
}
