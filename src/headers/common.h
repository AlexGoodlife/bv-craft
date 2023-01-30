#ifndef BV_COMMON_H
#define BV_COMMON_H

#include <stdio.h>



#define ERROR(MESSAGE, ...) do{fprintf(stderr,"EXITED AT %s ON LINE %d WITH ERROR MESSAGE: ", __FILE__, __LINE__); fprintf(stderr, MESSAGE, __VA_ARGS__); exit(-1);}while(0)


#endif