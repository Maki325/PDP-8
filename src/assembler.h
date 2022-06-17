#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct InstructionType {
  const char *name;
  size_t length;
  uint16_t mask;
} InstructionType;

typedef enum InstructionMask {
  INDIRECT_MASK = 0b1000000000000000,
  // Memory Instructions
  AND_MASK      = 0b0000000000000000,
  ADD_MASK      = 0b0001000000000000,
  LDA_MASK      = 0b0010000000000000,
  STA_MASK      = 0b0011000000000000,
  BUN_MASK      = 0b0100000000000000,
  BSA_MASK      = 0b0101000000000000,
  ISZ_MASK      = 0b0110000000000000,
  // Registry Instructions
  REG_MASK      = 0b0111000000000000,
  CLA_MASK      = 0b0111100000000000,
  CLE_MASK      = 0b0111010000000000,
  CMA_MASK      = 0b0111001000000000,
  CME_MASK      = 0b0111000100000000,
  CIR_MASK      = 0b0111000010000000,
  CIL_MASK      = 0b0111000001000000,
  INC_MASK      = 0b0111000000100000,
  SPA_MASK      = 0b0111000000010000,
  SNA_MASK      = 0b0111000000001000,
  SZA_MASK      = 0b0111000000000100,
  SZE_MASK      = 0b0111000000000010,
  HLT_MASK      = 0b0111000000000001,

  MASK_NONE,
} InstructionMask;

extern const InstructionType InstructionTypes[];

typedef struct Instruction {
  const InstructionType* type;
  bool indirect;
  int location;
  char *label;
  int value;
} Instruction;

typedef struct LabelLocation {
  char *label;
  int location;
} LabelLocation;

typedef struct Program {
  size_t location;

  Instruction *instructions;
  size_t count;
  size_t capacity;

  LabelLocation *labelLocations;
  size_t labelLocationsCount;
  size_t labelLocationsCapacity;
} Program;

Program *createProgram();
void expandProgramInstructions(Program *program);
void addProgramInstruction(Program *program, const InstructionType* type, bool indirect);

void tokenizeInstruction(Program *program, char *line, size_t start, size_t end, bool *isArgument, int *labelStep);
Program *tokenize(char *filePath);

void updateLocations(Program *program);

#endif // ASSEMBLER_H_
