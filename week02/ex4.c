#include <stdio.h>
#include <ctype.h>
void count (char string[], char s) {
    int i = 0;
    int counter = 0;
    while (string[i] != '\0') {
        if (string[i] == s) {
            counter++;
        }
        i++;
    }
    printf("%c: %d ", s, counter);

}

void countAll (char string[]) {
    for(int i = 0; string[i]; i++){
        string[i] = tolower(string[i]);
    }
    for (int i = 0; string[i]; i++) {
        count(string,string[i]);
    }
}
int main() {
    int n = 256;
    char string[n];
    char garbage;
    char aim;
    scanf("%s", string);
    countAll(string);
    return 0;
}
