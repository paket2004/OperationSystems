#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int main() {
    int p1[2];
    pipe(p1);
    int id = fork();
    if (id == 0) {
    	close(p1[1]);
    	char message[1024];
    	char symbol;
    	int i = 0;
    	 while (read(p1[0], &symbol, sizeof(char)) > 0 && i < sizeof(message) - 1)
        {
            message[i++] = symbol;
        }
        message[i] = '\0';
        printf("Message received: %s \n", message);
        close(p1[0]);
    } else {
    	close(p1[0]);
    	char message[1024];
    	fgets(message, sizeof message, stdin);
	for (int i = 0; i < strlen(message); i++)
        {
            if (write(p1[1], &message[i], sizeof(char)) == -1)
            {
                perror("write");
                return 1;
            }
        }
    	close(p1[1]);
    }
    return 0;
}
