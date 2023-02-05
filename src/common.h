#ifndef BV_COMMON_H
#define BV_COMMON_H

#include <stdio.h>

extern FILE *logFile;

#define LOG(MESSAGE,...) do{fprintf(logFile, MESSAGE, ##__VA_ARGS__);}while(0)

#define LOGLN(MESSAGE,...) do{fprintf(logFile, MESSAGE, ##__VA_ARGS__);fprintf(logFile, "\n");}while(0)

#define ERROR(MESSAGE, ...) do{fprintf(stderr,"EXITED AT %s ON LINE %d WITH ERROR MESSAGE: ", __FILE__, __LINE__); fprintf(stderr, MESSAGE, ##__VA_ARGS__); fprintf(stderr, " ERRNO: %s", strerror(errno));}while(0)

#define INDEX2D(x,y,width) ((y)*width) + (x)

#define INDEX3D(x,y,z,width,height) ((z)*width*height) + INDEX2D(x,y,width)

#endif