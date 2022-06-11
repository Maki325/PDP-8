#ifndef ARITHMETICS_H
#define ARITHMETICS_H

#define XOR(a, b) (a ^ b)
#define AND(a, b) (a & b)
#define OR(a, b) (a | b)

int add(int *a, int *b, int *out, int n);
int incOut(int *a, int *out, int n);
int inc(int *a, int n);
void and(int *a, int *b, int *out, int n);

int binToDec(int *bin, int n);

#endif // ARITHMETICS_H