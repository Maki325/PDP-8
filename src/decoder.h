#ifndef DECODER_H
#define DECODER_H

#include "includes.h"

void decoder2x4(int in[2], int out[4], bool enable);
void decoder3x8(int in[3], int out[8], bool enable);

#endif // DECODER_H