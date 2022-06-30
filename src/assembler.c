#include "assembler.h"
#include "includes.h"
#include "utils.h"
#include <string.h>
#include <ctype.h>

Program *createProgram() {
  Program *program = malloc(sizeof(Program));
  program->location = 0;

  program->capacity = 40;
  program->count = 0;
  program->instructions = calloc(program->capacity, sizeof(Instruction));
  
  program->labelLocationsCapacity = 40;
  program->labelLocationsCount = 0;
  program->labelLocations = calloc(program->labelLocationsCapacity, sizeof(LabelLocation));

  return program;
}

void expandProgramInstructions(Program *program) {
  program->capacity = program->capacity + min(1000, program->capacity);

  Instruction *instructions = program->instructions;
  program->instructions = calloc(program->capacity, sizeof(Instruction));
  for(size_t i = 0;i < program->count;i++) {
    program->instructions[i] = instructions[i];
  }
}

void addProgramInstruction(Program *program, const InstructionType* type, bool indirect) {
  if(program->count > program->capacity / 2) {
    expandProgramInstructions(program);
  }
  program->instructions[program->count].type = type;
  program->instructions[program->count].indirect = indirect;
  program->instructions[program->count].label = NULL;
  program->instructions[program->count].location = -1;
  program->instructions[program->count].value = 0;
  program->count++;
}

void expandProgramLabelLocations(Program *program) {
  program->labelLocationsCapacity = program->labelLocationsCapacity + min(1000, program->labelLocationsCapacity);

  LabelLocation *labelLocations = program->labelLocations;
  program->labelLocations = calloc(program->labelLocationsCapacity, sizeof(Instruction));
  for(size_t i = 0;i < program->labelLocationsCount;i++) {
    program->labelLocations[i] = labelLocations[i];
  }
}

void addProgramLabelLocations(Program *program, char *label, int location) {
  for(size_t i = 0;i < program->labelLocationsCount;i++) {
    if(strncmp(program->labelLocations[i].label, label, 255) == 0) return;
  }
  if(program->labelLocationsCount > program->labelLocationsCapacity / 2) {
    expandProgramLabelLocations(program);
  }
  program->labelLocations[program->labelLocationsCount].label = label;
  program->labelLocations[program->labelLocationsCount].location = program->location + location;
  program->labelLocationsCount++;
}

void tokenizeInstruction(Program *program, char *line, size_t start, size_t end, bool *isArgumentP, int *labelStepP) {
  if(*isArgumentP) {
    size_t len = end - start;
    if(isNumber(&line[start], len, false)) {
      program->instructions[program->count - 1].location = strToInt(&line[start], len);
    } else {
      program->instructions[program->count - 1].label = calloc(len + 1, sizeof(char));
      strncpy(program->instructions[program->count - 1].label, &line[start], len);
    }
    *isArgumentP = false;
  } else if(*labelStepP == 1) {
    if(strncmp(&line[start], "DEC", 3) == 0) {
      *labelStepP = 2;
    } else {
      *labelStepP = 3;
    }
  } else if(*labelStepP == 10) { // ORG broj
    size_t len = end - start;
    program->location = strToInt(&line[start], len);
    *labelStepP = 0;
  } else if(*labelStepP == 2) { // Decimal number
    size_t len = end - start;
    program->instructions[program->count - 1].value = strToInt(&line[start], len);
    *labelStepP = 0;
  } else if(*labelStepP == 3) { // Hexadecimal number
    size_t len = end - start;
    program->instructions[program->count - 1].value = hexToInt(&line[start], len);
    *labelStepP = 0;
  } else if(line[end - 1] == ',') {
    size_t len = end - start - 1;
    char *label = calloc(len + 1, sizeof(char));
    memcpy(label, &line[start], len);
    addProgramLabelLocations(program, label, program->count);
    addProgramInstruction(program, NULL, false);
    *labelStepP = 1;
  } else {
    for(size_t j = 0;j < 25;j++) {
      if(strncmp(&line[start], InstructionTypes[j].name, InstructionTypes[j].length) != 0) {
        continue;
      }
      if(InstructionTypes[j].mask > MASK_NONE) {
        if(InstructionTypes[j].mask == MASK_NONE + 4) {
          return;
        }
        if(InstructionTypes[j].mask == MASK_NONE + 1) {
          *labelStepP = 10;
          return;
        }
        addProgramInstruction(program, NULL, false);
        *labelStepP = InstructionTypes[j].mask - MASK_NONE;
        return;
      }
      bool indirect = false;
      if(line[end - 1] == '*') {
        indirect = true;
      }
      if((InstructionTypes[j].mask | REG_MASK) != InstructionTypes[j].mask) {
        *isArgumentP = true;
      }
      addProgramInstruction(program, &InstructionTypes[j], indirect);
    }
  }
}

Program *tokenize(char *filePath) {
  Program *program = createProgram();
  FILE *in = fopen(filePath, "r");

  char *lineStart = NULL;
  ssize_t lineLength = 0;
  size_t length = 0;
  size_t row = 0;

  bool isArgument = false;
  int labelStepP = 0;
  while((lineLength = getline(&lineStart, &length, in)) != -1) {
    row++;
    length = (size_t) lineLength;
    char *line = lineStart;
    trimRight(line, &length);
    if(length == 0) continue;
    trimLeft(&line, &length);

    size_t start = 0;
    for(size_t i = 0;i < length;i++) {
      if(i > 0 && line[i - 1] == ',' && !isspace(line[i])) {
        tokenizeInstruction(program, line, start, i, &isArgument, &labelStepP);
        start = i;
        continue;
      }
      if(!isspace(line[i])) continue;
      tokenizeInstruction(program, line, start, i, &isArgument, &labelStepP);
      start = i + 1;
    }
    tokenizeInstruction(program, line, start, length, &isArgument, &labelStepP);
  }

  fclose(in);

  return program;
}

void updateLocations(Program *program) {
  // for(size_t j = 0;j < program->labelLocationsCount;j++) {
  //   printf("Location[%zu]: %s - %d\n", j, program->labelLocations[j].label, program->labelLocations[j].location);
  // }

  for(size_t i = 0;i < program->count;i++) {
    Instruction *instruction = &program->instructions[i];
    if(!instruction->label) continue;
    for(size_t j = 0;j < program->labelLocationsCount;j++) {
      if(strcmp(instruction->label, program->labelLocations[j].label) != 0) continue;
      instruction->location = program->labelLocations[j].location;
      break;
    }
  }
}


// Variables
const InstructionType InstructionTypes[] = {
  {.name = "",    .length = 1, .mask = INDIRECT_MASK},
  {.name = "AND", .length = 3, .mask = AND_MASK},
  {.name = "ADD", .length = 3, .mask = ADD_MASK},
  {.name = "LDA", .length = 3, .mask = LDA_MASK},
  {.name = "STA", .length = 3, .mask = STA_MASK},
  {.name = "BUN", .length = 3, .mask = BUN_MASK},
  {.name = "BSA", .length = 3, .mask = BSA_MASK},
  {.name = "ISZ", .length = 3, .mask = ISZ_MASK},
  {.name = "REG", .length = 3, .mask = REG_MASK},
  {.name = "CLA", .length = 3, .mask = CLA_MASK},
  {.name = "CLE", .length = 3, .mask = CLE_MASK},
  {.name = "CMA", .length = 3, .mask = CMA_MASK},
  {.name = "CME", .length = 3, .mask = CME_MASK},
  {.name = "CIR", .length = 3, .mask = CIR_MASK},
  {.name = "CIL", .length = 3, .mask = CIL_MASK},
  {.name = "INC", .length = 3, .mask = INC_MASK},
  {.name = "SPA", .length = 3, .mask = SPA_MASK},
  {.name = "SNA", .length = 3, .mask = SNA_MASK},
  {.name = "SZA", .length = 3, .mask = SZA_MASK},
  {.name = "SZE", .length = 3, .mask = SZE_MASK},
  {.name = "HLT", .length = 3, .mask = HLT_MASK},

  {.name = "ORG", .length = 3, .mask = MASK_NONE + 1},
  {.name = "END", .length = 3, .mask = MASK_NONE + 4},

  {.name = "DEC", .length = 3, .mask = MASK_NONE + 2},
  {.name = "HEX", .length = 3, .mask = MASK_NONE + 3},
};
