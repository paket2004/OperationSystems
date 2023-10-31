#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

void allocateAndFillMemory(int sizeMB) {
    int sizeBytes = sizeMB * 1024 * 1024;
    char *memory = (char *)malloc(sizeBytes);
    if (memory == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    memset(memory, 0, sizeBytes);
    free(memory);
}

int main() {
    struct rusage usage;

    for (int i = 0; i < 10; i++) {
        printf("Allocating and filling 10 MB of memory...\n");
        allocateAndFillMemory(10);
        
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            printf("Memory usage (maximum resident set size): %ld KB\n", usage.ru_maxrss);
        } else {
            perror("Failed to get memory usage");
        }

        if (i < 9) {
            printf("Sleeping for 1 second...\n");
            sleep(1);
        }
    }

    return 0;
}
