#include <stdio.h>
#include <limits.h>
#include <float.h>
int main() {
    int a;
    unsigned short b;
    signed long int c;
    float d;
    double e;
    a = INT_MAX;
    b = USHRT_MAX;
    c = LONG_MAX;
    d = FLT_MAX;
    e = DBL_MAX;

    printf("value: %lu", a);
    printf(" size: %llu\n",sizeof (a));

    printf("value: %d", b);
    printf(" size: %llu\n",sizeof (b));

    printf("value: %d", c);
    printf(" size: %llu\n",sizeof (c));

    printf("value: %f", d);
    printf(" size: %llu\n",sizeof (d));

    printf("value: %f", e);
    printf(" size: %llu\n",sizeof (e));

    return 0;
}

