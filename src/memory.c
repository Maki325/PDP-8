#include "memory.h"

void copy(int *in, int *out, int n) {
 for(int i = 0;i < n;i++) {
   out[i] = in[i];
 }
}

void clear(int *mem, int n) {
 for(int i = 0;i < n;i++) {
   mem[i] = 0;
 }
}

void invert(int *mem, int n) {
 for(int i = 0;i < n;i++) {
   mem[i] = !mem[i];
 }
}

int shiftLeft(int *mem, int n) {
  if(n <= 0) return 0;
  int first = mem[0];
  for(int i = 0;i < n - 1;i++) {
    mem[i] = mem[i + 1];
  }
  return first;
}
int fullShiftLeft(int *mem, int n) {
  if(n <= 0) return 0;
  return mem[n - 1] = shiftLeft(mem, n);
}

int shiftRight(int *mem, int n) {
  if(n <= 0) return 0;
  int last = mem[n - 1];
  for(int i = n - 1;i > 0;i--) {
    mem[i] = mem[i - 1];
  }
  return last;
}
int fullShiftRight(int *mem, int n) {
  if(n <= 0) return 0;
  return mem[0] = shiftRight(mem, n);
}

// Variable defenitions

int PC[12];

int E;
int AC[WORD_SIZE];

int I;
int OPR[3];
int MAR[12];

// int OPERATION[WORD_SIZE];
// int *I = OPERATION;
// int *OPR = I + 1;
// int *MAR = OPR + 3;

int MBR[WORD_SIZE];

int RAM[4096 * WORD_SIZE];

int SC[2];
int F = 0;
int R = 0;
