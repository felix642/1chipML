#ifndef ONECHIPML_MULTI_ARM_BANDIT
#define ONECHIPML_MULTI_ARM_BANDIT

#define bandit_real double

#ifdef __cplusplus
extern "C" {
#endif

// a function that takes no arguments and returns a bandit_real
typedef bandit_real (*rewardFunctionType)(void);

bandit_real test(bandit_real (*rewardFunction)(void));
bandit_real test2(bandit_real (**rewardFunctions)(void));
bandit_real multiArmBandit(const unsigned int nArms, bandit_real* probs, const bandit_real epsilon, const unsigned int nIterations);
bandit_real multiArmBanditFunc(const unsigned int nArms, rewardFunctionType* probs, const bandit_real epsilon, const unsigned int nIterations);

#ifdef __cplusplus
}
#endif

#endif // ONECHIPML_MULTI_ARM_BANDIT

