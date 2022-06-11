#ifndef PDP_8_H
#define PDP_8_H
#include "includes.h"

void pdp_8_fetch();
void pdp_8_indirect();

void pdp_8_execute_and();
void pdp_8_execute_add();
void pdp_8_execute_lda();
void pdp_8_execute_sta();
void pdp_8_execute_bun();
void pdp_8_execute_bsa();
void pdp_8_execute_isz();

void pdp_8_execute_register();

void pdp_8_run();

void memoryDump();

extern int t[4], c[4], fr[2], q[8];
extern bool S;

typedef struct PDP_8 {
  int t[4];
  int c[4];
  int fr[2];
} PDP_8;

#endif // PDP_8_H