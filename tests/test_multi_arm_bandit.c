#include "../src/multi_arm_bandit.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    printf("multiArmBanditMain called\n");
    
    unsigned arms = 10;
    double* probs = malloc(arms * sizeof(double));
    double myArray[] = {0.10, 0.50, 0.60, 0.80, 0.10,
         0.25, 0.60, 0.45, 0.75, 0.65};
    for(int i = 0; i < arms; ++i) {
        probs[i] = myArray[i];
    }

    double epsilon = 0.01;
    unsigned nIterations = 100;
    multiArmBandit(arms, probs, epsilon, nIterations);

    free(probs);

    return 0;
}
