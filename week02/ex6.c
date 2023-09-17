#include <stdio.h>
int main() {
    for (int i = 0; i < 7;i++) {
        for (int j = 0; j < 7; j++) {
            printf("*");
        }
        printf("\n");
    }
    printf("---------------------------------------------");
    for (int i = 0; i <= 7;i++) {
        for (int j = 0; j < i; j++) {
            printf("*");
        }
        printf("\n");
    }
    printf("---------------------------------------------");
    for (int i = 0; i <= 8/2;i++) {
        for (int j = 0; j < i; j++) {
            printf("*");
        }
        printf("\n");
    }
    for (int i = 7/2; i >= 0 ;i--) {
        for (int j = i; j > 0; j--) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
