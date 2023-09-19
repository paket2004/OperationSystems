#include <stdio.h>
#include <stdbool.h>

void convert(long long int number, int source, int target) {
    bool flag = true;
    long long int newNumber = 0;

    if (source != 10) {
        flag = false;
        long long int temp = number;
        long long int multiplier = 1;

        while (temp > 0) {
            newNumber += (temp % 10) * multiplier;
            multiplier *= source;
            temp /= 10;
        }
    }

    if (flag) {
        newNumber = number;
    }

    if (newNumber == 0) {
        printf("0");
        return;
    }

    if (target == 1) {
        for (long long int i = 0; i < newNumber; i++) {
            printf("1");
        }
        return;
    }

    int numDigits = 0;
    long long int temp = newNumber;
    while (temp > 0) {
        temp /= target;
        numDigits++;
    }

    for (int i = numDigits - 1; i >= 0; i--) {
        int targetDigit = 0;
        long long int temp = newNumber;

        for (int j = 0; j < i; j++) {
            temp /= target;
        }

        targetDigit = temp % target;
        printf("%d", targetDigit);
    }
}

int main() {
    long long int number;
    int source;
    int target;

    scanf("%lld %d %d", &number, &source, &target);

    if (number < 0 || source < 2 || source > 10 || target < 2 || target > 10) {
        printf("Cannot convert!\n");
        return 0;
    }

    convert(number, source, target);
    return 0;
}
