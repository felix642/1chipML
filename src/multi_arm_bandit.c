#include "multi_arm_bandit.h"
#include <stdlib.h>
#include <stdio.h> // TODO: Remove this

typedef struct Bandit {
    unsigned int nArms;
    double* probsOfSuccess;
    double* armsMeanReward;
    unsigned int* armsStep;
    double epsilon;
} Bandit;

static unsigned int getChosenArm(const Bandit* const bandit, const unsigned int iteration) {

    if (bandit->epsilon == 0 || iteration == 0) {
        return rand() % bandit->nArms;
    }

    const double explore = (double)rand() / (double)RAND_MAX;
    if (explore < bandit->epsilon) {
        return rand() % bandit->nArms;
    }

    unsigned int maxIndex = 0;
    for (unsigned int i = 1; i < bandit->nArms; ++i) {
        if (bandit->armsMeanReward[i] > bandit->armsMeanReward[maxIndex]) {
            maxIndex = i;
        }
    }

    return maxIndex;
}

static double playArm(const unsigned int arm, Bandit* bandit) {
    // stochastic reward
    double rewardChance = (double)rand() / (double)RAND_MAX;
    if (rewardChance < bandit->probsOfSuccess[arm]) {
        return 1.0;
    }
    return 0.0;
}

static void updateArmResults(const double reward, const unsigned int arm, Bandit* bandit) {
    ++(bandit->armsStep[arm]);
    bandit->armsMeanReward[arm] += (reward - bandit->armsMeanReward[arm]) / bandit->armsStep[arm];
}

double multiArmBandit(const unsigned int nArms, double* probs, const double epsilon, const unsigned int nIterations) {

    srand(0);



    double armsMeanReward[nArms];
    unsigned int armsStep[nArms];
    Bandit bandit = {
        nArms,
        probs,
        armsMeanReward,
        armsStep,
        epsilon
    };

    for(unsigned int i = 0; i < nIterations; ++i) {
        unsigned int arm = getChosenArm(&bandit, i);
        double reward = playArm(arm, &bandit);
        updateArmResults(reward, arm, &bandit);
    }

    // normally get the most visited
    // But here get best result only for testing
    unsigned bestResultIndex = 0;
    for(unsigned i = 1; i < bandit.nArms; ++i) {
        if (bandit.armsMeanReward[i] > bandit.armsMeanReward[bestResultIndex]) {
            bestResultIndex = i;
        }
    }
    
    double bestResult = bandit.armsMeanReward[bestResultIndex];
    printf("Best result: %f\n", bestResult);
    printf("Best index: %d\n", bestResultIndex);

    bandit.armsMeanReward = NULL;
    bandit.armsStep = NULL;
    bandit.probsOfSuccess = NULL;

    return bestResult;
}
