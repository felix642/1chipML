#include "../src/gradient_descent.h"
#include <stdio.h>
#include <math.h>

// Function (x-4)^2 + (y+3)^2
double func(double* p) {
    return pow(p[0] - 4, 2) + pow(p[1] + 3, 2);
}

// Gradient of (x-4)^2 + 2xy - 4zx
void dfunc(double* p, double* grad) {
    grad[0] = 2 * (p[0] - 4);
    grad[1] = 2 * (p[1] + 3);
}

int main() {
    double min = 0;
    double p[2] = {3, 5};

    min = gradient_descent(func, dfunc, p, 2);
    
    printf("Minimizing function: (x-4)^2 + (y+3)^2\n");
    printf("The minimum of the function is %f\n", min);
    printf("X: %f\n", p[0]);
    printf("Y: %f\n", p[1]);
}