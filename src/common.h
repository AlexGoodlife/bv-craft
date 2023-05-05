#ifndef BV_COMMON_H
#define BV_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "math/vec.h"

extern FILE *logFile;

#define MIN(a, b) a < b ? a : b

#define MAX(a, b) a > b ? a : b

#define LOG(MESSAGE,...) do{fprintf(logFile, MESSAGE, ##__VA_ARGS__);}while(0)

#define LOGLN(MESSAGE,...) do{fprintf(logFile, MESSAGE, ##__VA_ARGS__);fprintf(logFile, "\n");}while(0)

#define LOG_VEC3(vec) LOGLN("x: %f, y: %f, z: %f", vec.x,vec.y,vec.z) 

#define LOG_IVEC3(vec) LOGLN("x: %d, y: %d, z: %d", vec.x,vec.y,vec.z) 

#define ERROR(MESSAGE, ...) do{fprintf(stderr,"EXITED AT %s ON LINE %d WITH ERROR MESSAGE: ", __FILE__, __LINE__); fprintf(stderr, MESSAGE, ##__VA_ARGS__); fprintf(stderr, " ERRNO: %s", strerror(errno));exit(1);}while(0)

#define TODO(MESSAGE, ...) do{fprintf(stderr,"EXITED AT %s ON LINE %d WITH TODO MESSAGE: ", __FILE__, __LINE__); fprintf(stderr, MESSAGE, ##__VA_ARGS__);exit(1);}while(0)

#define INDEX2D(x,y,width) ((y)* (width)) + (x)

#define INDEX3D(x,y,z,width,height) ((z)*width*height) + INDEX2D(x,y,width)

#define IN_BOUNDS(x,width) ((x) >= 0 && (x) < width) 

#define IN_BOUNDS_2D(x,y,width,height) ((x) >= 0 && (x) < width && (y) >= 0 && (y) < height)

#define IN_BOUNDS_3D(x,y,z,width,height,depth) IN_BOUNDS_2D(x,y,width,height) && ((z) >= 0 && (z) < depth)

#define NORMALIZE(original_value, original_min, original_max, target_min, target_max) (((original_value - original_min) / (original_max - original_min)) * (target_max - target_min)) + target_min

#define CLAMP(num, low_bound, high_bound) num < low_bound ? low_bound : num > high_bound ? high_bound : num

static inline int random_number(int lower, int upper){
    return (rand() % (upper-lower+1)) + lower;
}

#endif
