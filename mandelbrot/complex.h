#ifndef COMPLEX_MATH
#define COMPLEX_MATH

#include <math.h>

/* Complex number a+bi. */
struct complex_t {
  long double a; // real
  long double b; // imaginary
};

typedef struct complex_t complex; 

complex complex_add(complex x, complex y);
complex complex_sub(complex x, complex y);
complex complex_multiply(complex x, complex y);
long double complex_magnitude(complex x);

#endif
