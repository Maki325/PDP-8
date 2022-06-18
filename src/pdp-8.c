#include "pdp-8.h"
#include "decoder.h"
#include "memory.h"
#include "arithmetics.h"
#include "inttypes.h"
#include "utils.h"

void printArray(int *a, int n) {
  for(int i = 0;i < n;i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

void memoryDump(char *out) {
  FILE *f = fopen(out ? out : "dump.bin", "wb");
  for(int i = 0;i < 4096 * WORD_SIZE;i += 8) {
    uint8_t byte = 0;
    if(RAM[i + 0]) byte |= 1UL << 7;
    if(RAM[i + 1]) byte |= 1UL << 6;
    if(RAM[i + 2]) byte |= 1UL << 5;
    if(RAM[i + 3]) byte |= 1UL << 4;
    if(RAM[i + 4]) byte |= 1UL << 3;
    if(RAM[i + 5]) byte |= 1UL << 2;
    if(RAM[i + 6]) byte |= 1UL << 1;
    if(RAM[i + 7]) byte |= 1UL << 0;
    fwrite(&byte, sizeof(uint8_t), 1, f);
  }
  fclose(f);
}

void loadProgram(Program *program) {
  const int maxValue = 2048;
  int bits[16];
  
  for(size_t i = 0;i < program->count;i++) {
    Instruction *instruction = &program->instructions[i];
    if(instruction->type == NULL) {
      clear(bits, WORD_SIZE);
      getNumberBits(instruction->value, 12, &bits[4]);
      copy(bits, &RAM[(program->location + i) * WORD_SIZE], WORD_SIZE);
      continue;
    }
    uint16_t mask = instruction->type->mask;
    if(instruction->indirect) mask |= INDIRECT_MASK;
    if(instruction->location != -1)
      mask |= instruction->location > maxValue ? maxValue : instruction->location < -maxValue ? -maxValue : instruction->location;

    getNumberBits(mask, 16, bits);
    copy(bits, &RAM[(program->location + i) * WORD_SIZE], WORD_SIZE);
  }

  clear(bits, 16);
  getNumberBits(program->location, 16, bits);
  copy(bits, PC, 12);
  S = true;
}

void pdp_8_fetch() {
  if(t[0]) {
    copy(PC, MAR, 12);
  } else if(t[1]) {
    inc(PC, 12);
    int location = binToDec(MAR, 12);
    copy(&RAM[WORD_SIZE * location], MBR, WORD_SIZE);
  } else if(t[2]) {
    I = MBR[0];
    copy(&MBR[1], OPR, 3);
  } else if(t[3]) {
    if(q[7] && I) {
      R = 1;
    } else {
      F = 1;
    }
  }
}

void pdp_8_indirect() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    copy(&RAM[WORD_SIZE * location], MBR, WORD_SIZE);
  } else if(t[2]) {
    
  } else if(t[3]) {
    F = 1;
    R = 0;
  }
}

void pdp_8_execute_and() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    copy(&RAM[WORD_SIZE * location], MBR, WORD_SIZE);
  } else if(t[2]) {
    and(AC, MBR, AC, WORD_SIZE);
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_add() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    copy(&RAM[WORD_SIZE * location], MBR, WORD_SIZE);
  } else if(t[2]) {
    E = add(&AC[4], &MBR[4], &AC[4], 12);
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_lda() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    copy(&RAM[WORD_SIZE * location], AC, WORD_SIZE);
  } else if(t[2]) {
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_sta() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    copy(AC, &RAM[WORD_SIZE * location], WORD_SIZE);
  } else if(t[2]) {
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_bun() {
  if(t[0]) {
    copy(&MBR[4], PC, 12);
  } else if(t[1]) {
  } else if(t[2]) {
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_bsa() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    clear(&RAM[WORD_SIZE * location], 4);
    copy(PC, &RAM[WORD_SIZE * location], WORD_SIZE);
  } else if(t[2]) {
    incOut(MAR, PC, 12);
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_isz() {
  if(t[0]) {
    copy(&MBR[4], MAR, 12);
  } else if(t[1]) {
    int location = binToDec(MAR, 12);
    inc(&RAM[WORD_SIZE * location], WORD_SIZE);
  } else if(t[2]) {
    int location = binToDec(MAR, 12);
    if(isZero(&RAM[WORD_SIZE * location], WORD_SIZE)) {
      inc(PC, 12);
    }
  } else if(t[3]) {
    F = 0;
  }
}

void pdp_8_execute_register() {
  if(!t[3]) return;
  if(MBR[4]) {
    clear(AC, 16);
  } else if(MBR[5]) {
    E = 0;
  } else if(MBR[6]) {
    invert(AC, 16);
  } else if(MBR[7]) {
    E = !E;
  } else if(MBR[8]) {
    int last = shiftRight(AC, WORD_SIZE);
    AC[0] = E;
    E = last;
  } else if(MBR[9]) {
    int first = shiftLeft(AC, WORD_SIZE);
    AC[WORD_SIZE - 1] = E;
    E = first;
  } else if(MBR[10]) {
    inc(AC, WORD_SIZE);
  } else if(MBR[11]) {
    if(!AC[0])
      inc(PC, 12);
  } else if(MBR[12]) {
    if(AC[0])
      inc(PC, 12);
  } else if(MBR[13]) {
    if(isZero(AC, WORD_SIZE))
      inc(PC, 12);
  } else if(MBR[14]) {
    if(!E)
      inc(PC, 12);
  } else if(MBR[15]) {
    S = false;
  }
}

void pdp_8_run() {
  while(S) {
    decoder2x4(SC, t, S);

    fr[0] = F;
    fr[1] = R;
    decoder2x4(fr, c, true);
    decoder3x8(OPR, q, true);

    if(c[0]) {
      pdp_8_fetch();
    } else if(c[1]) {
      pdp_8_indirect();
    } else if(c[2]) {
      if(q[0]) {
        pdp_8_execute_and();
      } else if(q[1]) {
        pdp_8_execute_add();
      } else if(q[2]) {
        pdp_8_execute_lda();
      } else if(q[3]) {
        pdp_8_execute_sta();
      } else if(q[4]) {
        pdp_8_execute_bun();
      } else if(q[5]) {
        pdp_8_execute_bsa();
      } else if(q[6]) {
        pdp_8_execute_isz();
      } else if(q[7]) {
        pdp_8_execute_register();
      }
    } else if(c[3]) {}

    inc(SC, 2);
  }
}

// Variable defenitions
int t[4], c[4], fr[2], q[8];
bool S = false;
