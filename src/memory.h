#ifndef MEMORY_H
#define MEMORY_H
#include "includes.h"

#define WORD_SIZE 16

void copy(int *in, int *out, int n);
void clear(int *mem, int n);
void invert(int *mem, int n);

int shiftLeft(int *mem, int n);
int fullShiftLeft(int *mem, int n);

int shiftRight(int *mem, int n);
int fullShiftRight(int *mem, int n);

extern int PC[12];

extern int E;
extern int AC[WORD_SIZE];

extern int I;
extern int OPR[3];
extern int MAR[12];

// extern int OPERATION[WORD_SIZE];
// extern int *I = OPERATION;
// extern int *OPR = I + 1;
// extern int *MAR = OPR + 3;

extern int MBR[WORD_SIZE];

extern int RAM[4096 * WORD_SIZE];

extern int SC[2];
extern int F;
extern int R;

#endif //MEMORY_H
