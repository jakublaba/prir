#include "util.h"

int gcd(int a, int b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

int pow_i(int base, int exp) {
  int result = 1;
  while (exp--) {
    result *= base;
  }
  return result;
}
