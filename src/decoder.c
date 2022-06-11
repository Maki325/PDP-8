#include "decoder.h"

void decoder2x4(int in[2], int out[4], bool enable) {
  out[0] = enable && !in[0] && !in[1];
  out[1] = enable && !in[0] &&  in[1];
  out[2] = enable &&  in[0] && !in[1];
  out[3] = enable &&  in[0] &&  in[1];
}

void decoder3x8(int in[3], int out[8], bool enable) {
  out[0] = enable && !in[0] && !in[1] && !in[2];
  out[1] = enable && !in[0] && !in[1] &&  in[2];
  out[2] = enable && !in[0] &&  in[1] && !in[2];
  out[3] = enable && !in[0] &&  in[1] &&  in[2];
  out[4] = enable &&  in[0] && !in[1] && !in[2];
  out[5] = enable &&  in[0] && !in[1] &&  in[2];
  out[6] = enable &&  in[0] &&  in[1] && !in[2];
  out[7] = enable &&  in[0] &&  in[1] &&  in[2];
}