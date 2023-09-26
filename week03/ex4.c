#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <memory.h>

typedef void* (*Operation)(const void*, const void*);

void* add(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(double*)a + *(double*)b;
    return result;
}

void* multiply(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(double*)a * *(double*)b;
    return result;
}

void* max(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
    return result;
}

void* aggregate(void* base, size_t size, int n, void* initialValue, Operation opr) {
    void* result = malloc(sizeof(double));
    memcpy(result, initialValue, sizeof(double));

    for (int i = 0; i < n; i++) {
        void* temp = opr(result, (char*)base + i * size);
        free(result);
        result = temp;
    }

    return result;
}

void* multiplyInt(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(int*)a * *(int*)b;
    return result;
}

int main() {
    double doubleArray[] = {2.0, 3.0, 1.5, 5.5, 4.0};
    int intArray[] = {10, 3, 8, 5, 6};

    double initialAdd = 0.0;
    double initialMultiply = 1.0;
    double initialMaxDouble = -DBL_MAX;
    int initialMaxInt = -INT_MAX;
    int initialMultiplyInt = 1;

    double* resultAdd = aggregate(doubleArray, sizeof(double), 5, &initialAdd, add);
    printf("Addition Result (Double): %.2lf\n", *resultAdd);
    free(resultAdd);

    double* resultMultiply = aggregate(doubleArray, sizeof(double), 5, &initialMultiply, multiply);
    printf("Multiplication Result (Double): %.2lf\n", *resultMultiply);
    free(resultMultiply);

    double* resultMaxDouble = aggregate(doubleArray, sizeof(double), 5, &initialMaxDouble, max);
    printf("Max Result (Double): %.2lf\n", *resultMaxDouble);
    free(resultMaxDouble);

    int* resultMaxInt = aggregate(intArray, sizeof(int), 5, &initialMaxInt, max);
    printf("Max Result (Int): %d\n", *resultMaxInt);
    free(resultMaxInt);

    int* resultMultiplyInt = aggregate(intArray, sizeof(int), 5, &initialMultiplyInt, multiplyInt);
    printf("Multiplication Result (Int): %d\n", *resultMultiplyInt);
    free(resultMultiplyInt);

    int* resultAddInt = aggregate(intArray, sizeof(int), 5, &initialAdd, add);
    printf("Summation Result (Int): %d\n", *resultAddInt);
    free(resultAddInt);

    return 0;
}
