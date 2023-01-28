#ifndef ONECHIPML_FFT
#define ONECHIPML_FFT

#define fft_real double

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifdef __cplusplus
extern "C" {
#endif

int FFT(const unsigned length, fft_real* realArray, fft_real* imaginaryArray);

#ifdef __cplusplus
}
#endif

#endif // ONECHIPML_FFT