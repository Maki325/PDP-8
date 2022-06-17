#include "includes.h"
#include "pdp-8.h"
#include "assembler.h"
#include "utils.h"

int main(int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: %s <input file> [output file]\n", argv[0]);
    return 0;
  }
  Program *program = tokenize(argv[1]);
  updateLocations(program);

  char *outFile = NULL;
  if(argc >= 3) outFile = argv[2];

  loadProgram(program);
  pdp_8_run();
  memoryDump(outFile);
}
