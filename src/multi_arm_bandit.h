#ifndef ONECHIPML_MULTI_ARM_BANDIT
#define ONECHIPML_MULTI_ARM_BANDIT

#define bandit_real double

#ifdef __cplusplus
extern "C" {
#endif

typedef double (*rewardFunctionType)(void);

double test(double (*rewardFunction)(void));
double test2(double (**rewardFunctions)(void));
double multiArmBandit(const unsigned int nArms, double* probs, const double epsilon, const unsigned int nIterations);

#ifdef __cplusplus
}
#endif

#endif // ONECHIPML_MULTI_ARM_BANDIT

