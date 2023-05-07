#define FNL_IMPL
#include "worldgen.h"
#include "common.h"
#include <stdlib.h>

fnl_state noise;

void worldgen_init(){
  noise = fnlCreateState();
  noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
  noise.frequency = 0.005f;
  noise.fractal_type = FNL_FRACTAL_FBM;
  noise.octaves = 4;

}

static void fill_map_height(uint8_t* map,uint32_t width, uint32_t height, uint32_t x, uint32_t z, int max_height){
  enum BlockID to_fill = Stone;
  if(max_height <= 0.1f*height){
    to_fill = Water;
  }
  for(int i = 0; i < max_height;i++){
    if(i >= max_height/2 && to_fill != Water){
      to_fill = Dirt;
    }
    if(i == max_height-1 && to_fill != Water){
      if(max_height >= 0.40f*height){
        to_fill = Snow;
      }
      else
        to_fill = Grass;
    }
    map[INDEX3D(x,i,z,width,height)] = to_fill+1;
  }
}

uint8_t* worldgen_gen_chunk(vec3_s pos, uint32_t width, uint32_t height, uint32_t depth){
  uint8_t* result = calloc(depth * width * height,sizeof(uint8_t));
  float noise_data[width*depth];
  int index = 0;
  for (int y = 0; y < depth; y++)
  {
    for (int x = 0; x < width; x++) 
    {
      vec3_s noise_pos = vec3_add(pos, vec3(x,0,y));
      noise_data[index++] = fnlGetNoise2D(&noise, noise_pos.x,noise_pos.z);
    }
  }

  for (int y = 0; y < depth; y++)
  {
    for (int x = 0; x < width; x++) 
    {
      float noise_value = noise_data[INDEX2D(x,y,width)];
      noise_value = (noise_value+1)/2;
      noise_value = pow(noise_value, 4.2069f);
      noise_value = round(noise_value*84)/84.0f;
      noise_value = NORMALIZE(noise_value, 0.0f,1.0f,0.1f,0.90f);
      // noise_value = CLAMP(noise_value,0.10f,0.75f);
      int max_height = height* noise_value;
      fill_map_height(result,width,height,x,y,max_height);
    }
  }
  
  return result;
}

uint8_t * worldgen_random(uint32_t width, uint32_t height, uint32_t depth){
  uint8_t* result = calloc(depth * width * height,sizeof(uint8_t)) ;
  for(int i = 0; i < depth;i++){
    for(int j = 0; j < width;j++){
      int rand_height = random_number(height/2,height);
      for(int y = 0; y < rand_height;y++){
        result[INDEX3D(j, y, i, width, height)] = Grass+1;
      }
      
    }
  }
  return result;
}
