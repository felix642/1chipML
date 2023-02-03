#include "multi_arm_bandit.h"
#include <stdlib.h>
#include <stdio.h> // TODO: Remove this

typedef struct Bandit {
    unsigned int nArms;
    bandit_real* probsOfSuccess;
    bandit_real* armsMeanReward;
    unsigned int* armsStep;
    bandit_real epsilon;
} Bandit;

static unsigned int getChosenArm(const Bandit* const bandit, const unsigned int iteration) {

    if (bandit->epsilon == 0 || iteration == 0) {
        return rand() % bandit->nArms;
    }

    const bandit_real explore = (bandit_real)rand() / (bandit_real)RAND_MAX;
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

static bandit_real playArm(const unsigned int arm, Bandit* bandit) {
    // TODO: Modify

    bandit_real rewardChance = (bandit_real)rand() / (bandit_real)RAND_MAX;
    if (rewardChance < bandit->probsOfSuccess[arm]) {
        return 1.0;
    }
    return 0.0;
}

static void updateArmResults(const bandit_real reward, const unsigned int arm, Bandit* bandit) {
    ++(bandit->armsStep[arm]);
    bandit->armsMeanReward[arm] += (reward - bandit->armsMeanReward[arm]) / bandit->armsStep[arm];
}

// passing single function
bandit_real test(bandit_real (*rewardFunction)(void)) {
    printf("passing func as arg: %f \n", rewardFunction());
    return 0.0;
}

// passing multiple functions
bandit_real test2(bandit_real (**rewardFunctions)(void)) {
    printf("passing array of func as arg: %f \n", rewardFunctions[0]());
    return 0.0;
}

bandit_real multiArmBandit(const unsigned int nArms, bandit_real* probs, const bandit_real epsilon, const unsigned int nIterations) {

    srand(0);

    bandit_real armsMeanReward[nArms];
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
        bandit_real reward = playArm(arm, &bandit);
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
    
    bandit_real bestResult = bandit.armsMeanReward[bestResultIndex];
    printf("Best result: %f\n", bestResult);
    printf("Best index: %d\n", bestResultIndex);

    bandit.armsMeanReward = NULL;
    bandit.armsStep = NULL;
    bandit.probsOfSuccess = NULL;

    return bestResult;
}
