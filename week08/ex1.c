#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
 	srand(time(NULL));
	FILE *urandom = fopen("/dev/urandom", "r");
    if (!urandom) {
        perror("Failed to open /dev/urandom");
        return 1;
    }

    char password[13] = "pass:";
    int i = 5;

    while (i < 13) {
        unsigned char random_char;
        if (fread(&random_char, 1, 1, urandom) != 1) {
            perror("Failed to read from /dev/urandom");
            fclose(urandom);
            return 1;
        }
        if (isprint(random_char)) {
            password[i] = (char)random_char;
            i++;
        }
    }

    fclose(urandom);
    password[13] = '\0';
	FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (!pid_file) {
        perror("Failed to open /tmp/ex1.pid");
        return 1;
    }
    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);
    printf("Generated Password: %s\n", password);
    char *mmap_pas = (char *)mmap(0, 14, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap_pas == MAP_FAILED) {
        perror("Failed to mmap");
        return 1;
    }
strcpy(mmap_pas, password);
sleep(10);
while(1) {
	sleep(1);
	}

return 0;
}
