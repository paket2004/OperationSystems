#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <signal.h>
int access_num = 0;
int fd;
const char* algorithm;
int* page_reference_count;
size_t page_table_size;
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE* page_table;
int pages_num;
int frames_num;

void printPTE(struct PTE* page_table, int pages_num) {
    for (int i = 0; i < pages_num; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
}
void printRAM(char RAM[][9], int rows) {
    for (int i = 0; i < rows; i++) {
        printf("Frame %d ---> ", i);
        for (int j = 0; j < 8; j++) {
            printf("%c", RAM[i][j]); // Adjust the format as needed
        }
        printf("\n"); // Move to the next row
    }
}

void printDISK(char disk[][9], int rows) {
    printf("Disk array \n");
    for (int i = 0; i < rows; i++) {
        printf("Page %d ---> ", i);
        for (int j = 0; j < 8; j++) {
            printf("%c", disk[i][j]); // Adjust the format as needed
        }
        printf("\n"); // Move to the next row
    }
}

char (*RAM)[9];
char (*disk)[9];
int randAlgo(struct PTE* page_table, int i) {
	int r = rand() % frames_num;
    for (int k = 0; k < pages_num; k++) {
      						if (r == page_table[k].frame) {
      							if (page_table[k].dirty == 1) {
      								page_table[k].dirty = 0;
      								}
      						
      						strcpy(disk[k],RAM[r]);
      						page_table[k].frame = -1;
      						page_table[k].valid = 0;
      						break;
      					}
      				}
      				
      				printf("We do not have free frames in RAM\n");
      				printf("Chose a random victim page %d\n",r);
      				printf("Replace/Evict it with page %d to be allocated to frame %d\n", i,r);
      				printf("Copy data from the disk (page = %d) to RAM (frame = %d)",i,r);
      				strcpy(RAM[r],disk[i]);
      				page_table[i].valid = 1;
      				page_table[i].frame = r;
	return r;
}
int nfuAlgo(struct PTE* page_table, int i) {
	int min_count = page_reference_count[0];
    int min_index = 999;

    for (int i = 1; i < pages_num; i++) {
        if ((page_reference_count[i] < min_count) && (page_table[i].frame != -1) && page_table[i].valid != 0) {
            min_count = page_reference_count[i];
            min_index = i;
        }
    }
    // Replace the page with the least references
    
    int r = page_table[min_index].frame;
    for (int k = 0; k < pages_num; k++) {
      						if (r == page_table[k].frame) {
      							if (page_table[k].dirty == 1) {
      								page_table[k].dirty = 0;
      								}
      						strcpy(disk[k],RAM[r]);
      						page_table[k].frame = -1;
      						page_table[k].valid = 0;
      						break;
      					}
      				}
      				
      				printf("We do not have free frames in RAM\n");
      				printf("Chose a random victim page %d\n",r);
      				printf("Replace/Evict it with page %d to be allocated to frame %d\n", i,r);
      				printf("Copy data from the disk (page = %d) to RAM (frame = %d)",i,r);
      				strcpy(RAM[r],disk[i]);
      				page_table[i].valid = 1;
      				page_table[i].frame = r;
      				page_reference_count[min_index] = 0;
	return r;
}
void signalHandler(int signum) {
	bool finish = false;
	int index = 0;
	if (signum == SIGUSR1) {
       for (int i = 0; i < pages_num; i++) {
      		if (page_table[i].referenced != 0) {
      			index = i;
      			page_reference_count[i]++;
      			printf("A disk access request from MMU Process (pid = %d) \n",page_table[i].referenced);
      			printf("Page %d is referenced\n", i);
      			bool flag = true;
      			bool flag2 = false;
      			for (int j = 0; j < frames_num; j++) {
      				if ((strlen(RAM[j]) == 0) && flag == true) {
      					printf("We can allocate it to free frame %d\n", j);
      					printf("Copy data from the disk (page = %d) to RAM (frame = %d)\n", i,j);
      					strcpy(RAM[j],disk[i]);
      					flag = false;
      					flag2 = true;
     					page_table[i].valid = 1;
     					page_table[i].frame = j;
      					}
      			}
      			if (flag2 == false) {
      				if (strcmp("random",algorithm) == 0) {
      					int r = randAlgo(page_table,i);
      				} else if (strcmp("nfu",algorithm) == 0){
      					int r = nfuAlgo(page_table,i);
      				}
      			
      			}
      			finish = true; 
      		}    
      		
       }
       if (finish == false) {
       	remove("pagetable2");
       	munmap(page_table,page_table_size);
       	printf("Access number: %d\n",access_num);
       	exit(0);
       }
    printf("RAM array\n");
    printRAM(RAM,frames_num);
    access_num++;
    printf("disk accesses is %d so far\n", access_num);
    printf("Resume mmu process\n");
    printf("-------------------\n");
    kill(page_table[index].referenced,SIGCONT);
    page_table[index].referenced = 0;
    }
}
int main(int argc, char* argv[]) {
    printf("Pager PID %d\n", getpid());
    if (argc != 4) {
        printf("Usage: %s <number of pages> <number of frames>\n", argv[0]);
        return 1;
    }
    signal(SIGUSR1, signalHandler);
    pages_num = atoi(argv[1]);
    frames_num = atoi(argv[2]);
    algorithm = argv[3];
	printf("%s\n", algorithm);
    srand(time(NULL));
	FILE *urandom = fopen("/dev/urandom", "r");
    if (!urandom) {
        perror("Failed to open /dev/urandom");
        return 1;
    }
	unsigned char random_char;
	RAM = malloc(frames_num * sizeof(*RAM));
    disk = malloc(pages_num * sizeof(*disk));
	memset(RAM,0, sizeof(RAM));
	int j = 0;
	
	page_reference_count = (int*)malloc(pages_num * sizeof(int));
	for (int i = 0; i < pages_num; i++) {
    	page_reference_count[i] = 0;
	}
	for (int i = 0; i < pages_num; i++) {
		while (j < 8) {
			if (fread(&random_char, 1, 1, urandom) != 1) {
            perror("Failed to read from /dev/urandom");
            fclose(urandom);
            return 1;
        	}
        	if (isprint(random_char)) {
            	disk[i][j] = (char)random_char;
        		j++;
        	}
		}
		j = 0;
		disk[i][8] = '\0';
	}
	
    // Open the file with appropriate permissions
    fd = open("pagetable2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Unable to create/open the file");
        exit(1);
    }

    page_table_size = pages_num * sizeof(struct PTE);

    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Unable to create page table");
        close(fd);
        exit(1);
    }

    for (int i = 0; i < pages_num; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }
	printf("Initialized page table\n");
	printPTE(page_table, pages_num);
	printf("-----------------------\n");
	printf("Initialized RAM\n");
    printRAM(RAM,frames_num);
    printf("-----------------------\n");
    printf("Initialized disk \n");
    printDISK(disk,pages_num);
    
    while(1) {
    
    }
    
    close(fd);

    return 0;
}
