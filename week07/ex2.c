#define _GNU_SOURCE // To use mmap
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

#define FILE_SIZE (500 * 1024 * 1024)
#define CHUNK_SIZE (1024 * sysconf(_SC_PAGESIZE))

int main() {
    int dev_random_fd = open("/dev/random", O_RDONLY);
    if (dev_random_fd == -1) {
        perror("Error opening /dev/random");
        return 1;
    }

    int text_fd = open("text.txt", O_CREAT | O_WRONLY, 0666);
    if (text_fd == -1) {
        perror("Error opening text.txt");
        return 1;
    }

    char* text = (char*)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, 0);
    if (text == MAP_FAILED) {
        perror("Error mapping text.txt");
        return 1;
    }

    int capital_count = 0;
    int current_line_length = 0;

    while (current_line_length < CHUNK_SIZE) {
        char c;
        if (read(dev_random_fd, &c, 1) != 1) {
            perror("Error reading /dev/random");
            return 1;
        }

        if (isprint(c)) {
            text[current_line_length] = c;
            if (isupper(c)) {
                capital_count++;
                text[current_line_length] = tolower(c);
            }
            current_line_length++;
        }

        if (current_line_length % 1024 == 0) {
            text[current_line_length] = '\n';
            current_line_length++;
        }
    }

    printf("Total capital letters in the file: %d\n", capital_count);

    if (munmap(text, FILE_SIZE) == -1) {
        perror("Error unmapping text.txt");
        return 1;
    }

    close(text_fd);
    close(dev_random_fd);

    return 0;
}
