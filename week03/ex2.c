#include <stdio.h>
#include <math.h>

struct point {
    double x;
    double y;
};
double distance (struct point p1, struct point p2) {
    double distance;
    distance = pow(pow(p2.x - p1.x,2) + pow(p2.y - p1.y,2),0.5);
    return distance;
}

double area (struct point p1, struct point p2, struct point p3) {
    double area = 0.5 * (p1.x * p2.y - p2.x * p1.y + p2.x * p3.y -p3.x  * p2.y + p3.x * p1.y - p1.x * p3.y);
    return area;
}
int main() {
    struct point A;
    A.x = 2.5;
    A.y = 6;

    struct point B;
    B.x = 1;
    B.y = 2.2;

    struct point C;
    C.x = 10;
    C.y = 6;

    printf(" %f \n", distance(A,B));
    printf("%f ", area(A,B,C));
    return 0;
}
