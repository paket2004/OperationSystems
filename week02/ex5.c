#include <stdio.h>
int tribonacci (int n) {
    long long int var0 = 0;
    long long int var1 = 1;
    long long int var2 = 1;
    long long int res;
    for (int i = 2; i < n;i++) {
        res = var0 + var1 + var2;
        var0 = var1;
        var1 = var2;
        var2 = res;
    }
    return res;
}
int main() {
    int n;
    scanf("%d", &n);
    printf("%d", tribonacci(n));
    return 0;
}
