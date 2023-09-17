#include <stdio.h>
#include <string.h>
int main() {
    char string[256];
    char currentCharacter;
    int i = 0;
    while (1) {
        scanf("%c", &currentCharacter);
        if (currentCharacter == '.') {
            string[i] = '\0';
            break;
        }
        string[i] = currentCharacter;
        i++;
    }
    printf("\"");
    printf("%s", strrev(string));
    printf("\"");
    return 0;
}
