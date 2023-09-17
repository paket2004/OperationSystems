#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void convert (long long int number, int source, int target) {
    int n = 200;
    int ans[n];
    bool flag = true;
    long long int newNumber = 0;
    if (source != 10) {
        flag = false;
        long long int temp = number;
        int i = 0;
        while (temp > 0) {
            newNumber += temp%10 * pow(source,i);
            i++;
            temp /= 10;
        }
    }
    if (flag) {
        newNumber = number;
    }
    int i = 0;
    while (newNumber > 0) {
        ans[i] = newNumber%target;
        newNumber /= target;
        i++;
    }
    if (i == 0) {
        printf("0");
    } else {
        for (int j = i - 1; j >= 0; j--) {
            printf("%d", ans[j]);
        }
    }

}
int main() {
    long long int number;
    int source;
    int target;
    scanf("%lld %d %d", &number,&source,&target);
    if (number < 0 || source < 2 || source > 10 || target < 2 || target > 10) {
        printf("cannot convert!");
        return 0;
    }
    convert(number,source,target);
    return 0;
}
