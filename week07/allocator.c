#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEM_SIZE 10000000

// Global memory array
unsigned int memory[MEM_SIZE];

void initializeMemory() {
    for (int i = 0; i < MEM_SIZE; i++) {
        memory[i] = 0;
    }
}

int FirstFit(unsigned int adrs, int size) {
    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            i++;
        }
        if (block_size >= size) {
            i = i - block_size; // Reposition i to the start of the block
            for (int j = i; j < i + size; j++) {
                memory[j] = adrs;
            }
            return 1;
        }
    }
    return 0;
}

int BestFit(unsigned int adrs, int size) {
    int index = -1;
    int size2 = MEM_SIZE + 1;

    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            i++;
        }
        if (block_size >= size && block_size < size2) {
            size2 = block_size;
            index = i - block_size;
        }
    }

    if (index >= 0) {
        for (int j = index; j < index + size; j++) {
            memory[j] = adrs;
        }
        return 1;
    }
    return 0;
}

int WorstFit(unsigned int adrs, int size) {
    int index = -1;
    int size2 = 0;

    for (int i = 0; i < MEM_SIZE; i++) {
        int block_size = 0;
        while (i < MEM_SIZE && memory[i] == 0) {
            block_size++;
            i++;
        }
        if (block_size >= size && block_size > size2) {
            size2 = block_size;
            index = i - block_size;
        }
    }

    if (index >= 0) {
        for (int j = index; j < index + size; j++) {
            memory[j] = adrs;
        }
        return 1;
    }
    return 0;
}

void clearMemory(unsigned int adrs) {
    for (int i = 0; i < MEM_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

int main() {
    initializeMemory();
    FILE* file = fopen("queries.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open queries.txt\n");
        return 1;
    }

    char line[100];
    int totalQueries = 0;
    clock_t startTime = clock();

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "allocate", 8) == 0) {
            unsigned int adrs;
            int size;
            sscanf(line, "allocate %u %d", &adrs, &size);
			      //uncomment to use another method
            if (WorstFit(adrs, size)) totalQueries++;
            //if (FirstFit(adrs, size)) totalQueries++;
            //if (BestFit(adrs, size)) totalQueries++;
            
        } else if (strncmp(line, "clear", 5) == 0) {
            unsigned int adrs;
            sscanf(line, "clear %u", &adrs);
            clearMemory(adrs);
            totalQueries++;
        }
    }

    fclose(file);

    clock_t endTime = clock();
    double executionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("Total Queries: %d\n", totalQueries);
    printf("Total Allocation Time: %.6f seconds\n", executionTime);

    if (executionTime > 0) {
        double throughput = (double)totalQueries / executionTime;
        printf("Throughput: %.6f queries per second\n", throughput);
    }
	return 0;
}
