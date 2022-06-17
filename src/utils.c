#include "utils.h"
#include <ctype.h>

void printn(const char* string, size_t length) {
  for(size_t i = 0;i < length;i++) {
    fputc(string[i], stdout);
  }
}

int trimLeft(char **text, size_t *length) {
  size_t i = 0;
  while(isspace((*text)[0]) && *length) {
    (*text)++;
    (*length)--;
    i++;
  }
  return i;
}
int trimRight(const char *text, size_t *length) {
  if(*length == 0) return 0;
  size_t i = 0;
  while(isspace(text[*length - 1]) && *length) {
    (*length)--;
    i++;
  }
  return i;
}

bool isNumber(char *string, size_t len) {
  size_t start = string[0] == '-' ? 1 : 0;
  for(size_t i = start;i < len;i++) {
    if(string[i] > '9' || string[i] < '0') {
      if(string[i] > 'f' || string[i] < 'a') {
        if(string[i] > 'F' || string[i] < 'A') {
          return false;
        }
      }
    }
  }
  return true;
}

int strToInt(char *string, size_t len) {
  int number = 0;
  size_t start = string[0] == '-' ? 1 : 0;
  for(size_t i = start;i < len;i++) {
    number = number * 10 + string[i] - '0';
  }
  if(start == 1) number = -number;
  return number;
}

int hexToInt(char *string, size_t len) {
  int number = 0;
  size_t start = string[0] == '-' ? 1 : 0;
  for(size_t i = start;i < len;i++) {
    if('0' <= string[i] && '9' >= string[i]) {
      number = number * 16 + string[i] - '0';
    } else {
      if(string[i] == 'a' || string[i] == 'A') {
        number = number * 16 + 10;
      } else if(string[i] == 'b' || string[i] == 'B') {
        number = number * 16 + 11;
      } else if(string[i] == 'c' || string[i] == 'C') {
        number = number * 16 + 12;
      } else if(string[i] == 'd' || string[i] == 'D') {
        number = number * 16 + 13;
      } else if(string[i] == 'e' || string[i] == 'E') {
        number = number * 16 + 14;
      } else if(string[i] == 'f' || string[i] == 'F') {
        number = number * 16 + 15;
      }
    }
  }
  if(start == 1) number = -number;
  return number;
}

int min(int a, int b) {
  return a < b ? a : b;
}

void printProgram(Program *program) {
  printf("program->count: %zu\n", program->count);
  for(size_t i = 0;i < program->count;i++) {
    Instruction *instruction = &program->instructions[i];
    if(instruction->type == NULL) {
      printf("Instruction {memoryLocation: %zu, value: %d}\n", i, instruction->value);
    } else {
      printf("Instruction {memoryLocation: %zu, name: \"%s\", indirect: %d, location: %d, label: \"%s\"}\n", i, instruction->type->name, instruction->indirect, instruction->location, instruction->label);
    }
  }
}

void getNumberBits(int number, int bitswanted, int *out) {
  for(int k = 0;k < bitswanted;k++){
    int mask =  1 << k;
    int masked_n = number & mask;
    int thebit = masked_n >> k;
    out[bitswanted - 1 - k] = thebit;
  }
}
