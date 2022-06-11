#include "arithmetics.h"

int add(int *a, int *b, int *out, int n) {
  int carry = 0;
  for(int i = n - 1; i >= 0; i--) {
    int x = a[i], y = b[i];
    out[i] = XOR(XOR(x, y), carry);
    carry = OR(AND(XOR(x, y), carry), AND(x, y));
  }

  return carry;
}

int inc(int *a, int n) {
  return incOut(a, a, n);
}

int incOut(int *a, int *out, int n) {
  int b[n];

  for(int i = 0; i < n - 1; i++) {
    b[i] = 0;
  }
  b[n - 1] = 1;

  return add(a, b, out, n);
}

void and(int *a, int *b, int *out, int n) {
  for(int i = 0;i < n;i++) {
    out[i] = AND(a[i], b[i]);
  }
}

int binToDec(int *bin, int n) {
  int dec = 0;
  for(int i = 0;i < n;i++) {
    dec = dec * 2 + bin[i];
  }
  return dec;
}

bool isZero(int *mem, int n) {
  for(int i = 0;i < n;i++) {
    if(mem[i] != 0) {
      return false;
    }
  }
  return true;
}
