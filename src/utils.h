#ifndef UTILS_H_
#define UTILS_H_
#include "includes.h"
#include "assembler.h"

void printn(const char* string, size_t length);

int trimLeft(char **text, size_t *length);
int trimRight(const char *text, size_t *length);

bool isNumber(char *string, size_t len);
int strToInt(char *string, size_t len);
int hexToInt(char *string, size_t len);

int min(int a, int b);

void printProgram(Program *program);

void getNumberBits(int number, int bitswanted, int *out);

#endif // UTILS_H_
