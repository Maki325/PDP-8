#include "includes.h"
#include "pdp-8.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: %s <input file>\n", argv[0]);
    return 0;
  }
  loadProgram(argv[1]);
  pdp_8_run();
  memoryDump();
}
