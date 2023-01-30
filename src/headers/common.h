#ifndef BV_COMMON_H
#define BV_COMMON_H

#include <stdio.h>
#include <errno.h>
#include <string.h>


#define ERROR(MESSAGE, ...) do{fprintf(stderr,"EXITED AT %s ON LINE %d WITH ERROR MESSAGE: ", __FILE__, __LINE__); fprintf(stderr, MESSAGE, ##__VA_ARGS__); fprintf(stderr, "ERRNO: %s", strerror(errno));}while(0)


#endif