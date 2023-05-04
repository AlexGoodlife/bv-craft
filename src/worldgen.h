#ifndef BVCRAFT_WORLDGEN_H
#define BVCRAFT_WORLDGEN_H

#include "Block.h"
#include "../lib/FastNoiseLite/FastNoiseLite.h"
void worldgen_init();
uint8_t* worldgen_gen_chunk(vec3_s pos, uint32_t width, uint32_t height, uint32_t depth);
uint8_t * worldgen_random(uint32_t width, uint32_t height, uint32_t depth);
#endif
