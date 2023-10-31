#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};
struct PTE* page_table;
void printPTE(struct PTE* page_table, int pages_num) {
    printf("Page table \n");
    for (int i = 0; i < pages_num; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <number of pages> <PID> <access list...>\n", argv[0]);
        return 1;
    }
    int pages_num = atoi(argv[1]);
    int pager_pid = atoi(argv[argc - 1]);
    // Open the file with appropriate permissions
    int fd = open("pagetable2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Unable to create/open the file");
        exit(1);
    }

    size_t page_table_size = pages_num * sizeof(struct PTE);

    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }
	page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Initialized page table \n");
	printPTE(page_table, pages_num);
	
	for (int i = 2; i < argc - 1; i++) {
        char* access = argv[i];
        char mode = access[0];
        int page = atoi(access + 1);
		//printf("%c %c\n",access[0],access[1]);
        switch (mode) {
            case 'R':
                if (page_table[page].valid == 0) {
              		printf("Read request for page %d\n", page);
              		printf("It is not a valid page --> page fault \n");
              		printf("Ask pager to load it from disk (SIGUSR1 signal) and wait \n");
              		page_table[page].referenced = getpid();
              		kill(pager_pid,SIGUSR1);
              		kill(getpid(), SIGSTOP);
              		printf("MMU resumed by SIGCONT signal from pager\n");
              		printPTE(page_table, pages_num);
              		printf("---------------------\n");
              		break;
                }
                if (page_table[page].valid == 1) {
                	printf("Read request for page %d\n", page);
                	printf("It is a valid page\n");
                	printPTE(page_table, pages_num);
              		printf("---------------------\n");
              		break;
                }
            case 'W':
                printf("Write request for page %d\n", page);
                if (page_table[page].valid == 0) {
              		printf("It is not a valid page --> page fault \n");
              		printf("Ask pager to load it from disk (SIGUSR1 signal) and wait \n");
              		page_table[page].referenced = getpid();
              		kill(pager_pid,SIGUSR1);
              		kill(getpid(), SIGSTOP);
              		printf("MMU resumed by SIGCONT signal from pager\n");
              		printPTE(page_table, pages_num);
              		printf("---------------------\n");
                } else {
                printf("It is a valid page\n");
               	printf("It is a write request then set the dirty field\n");
           		page_table[page].dirty = 1;
           		printPTE(page_table, pages_num);
              	printf("---------------------\n");
                }
                break;
            default:
                printf("Unknown access: %s\n", access);
                break;
        }
    }

    close(fd);
	munmap(page_table,page_table_size);
	kill(pager_pid,SIGUSR1);
    return 0;
}
