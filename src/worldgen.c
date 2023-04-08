#include "worldgen.h"
#include "common.h"
#include <stdlib.h>

uint32_t * worldgen_random(uint32_t width, uint32_t height, uint32_t depth){
  uint32_t* result = malloc(sizeof(uint32_t) * depth * width * height);
  for(int i = 0; i < depth;i++){
    for(int j = 0; j < width;j++){
      int rand_height = random_number(0,height);
      for(int y = 0; y < rand_height;y++){
        result[INDEX3D(j, y, i, width, height)] = Grass;
      }
      
    }
  }
  return result;
}
