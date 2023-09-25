#include <stdio.h>
#include <malloc.h>

long long int const_tri(int* p, int n) {
    if (n == 0) {
        return 0;
    }
    else if (n == 1 || n == 2) {
        return 1;
    }
    else {
        return const_tri(p,n - 1) + const_tri(p,n - 2) + const_tri(p,n - 3);
    }

}
int main() {
    const int x = 1;
    int *q = &x;
    int *p = NULL;
    p = (int*)  malloc(3 * sizeof(int));
    *p = x;
    p++;
    *p = x;
    p++;
    *p = 2 * x;
    p -= 2;
//    work with the vфlue of p to be able to print the fact that cells are continuous and after that reallocate memory
    printf("%p\n", p);
    p++;
    printf("%p\n", p);
    p++;
    printf("%p\n", p);
    p -= 2;
    free(p);
    printf("%llu",const_tri(p,7));
    return 0;
}
