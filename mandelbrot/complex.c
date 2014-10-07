#include "complex.h"

/* 
 * Adds to complex numbers and returns the result.
 */
complex complex_add(complex x, complex y)
{
  complex result;
  result.a = x.a + y.a;
  result.b = x.b + y.b;
  return result;
}

/* 
 * Subtracts two complex numbers and returns the result.
 */
complex complex_sub(complex x, complex y)
{
  complex result;
  result.a = x.a - y.a;
  result.b = x.b - y.b;
  return result;
}

/* 
 * Multiplies two complex numbers and returns the result.
 */
complex complex_multiply(complex x, complex y)
{
  complex result;
  result.a = x.a * y.a - x.b * y.b;
  result.b = x.a * y.b + y.a * x.b;
  return result;
}

/*
 * Calculates the complex magnitude of x and returns the result.
 */
long double complex_magnitude(complex x)
{
  return sqrtl(x.a * x.a + x.b * x.b);
}
