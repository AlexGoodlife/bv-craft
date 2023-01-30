#include "headers/Shader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024

char *concact_strings(const char*s1, const char*s2){
    uint32_t firstSize = strlen(s1);
    uint32_t secondSize = strlen(s2);
    char* result = malloc(sizeof(char) * (firstSize + secondSize) + 1);
    strcat(result, s1);
    strcat(result,s2);
    return result;
}


Shader_id loadShader(const char* vertexPath, const char* fragPath){
    errno = 0;
    FILE* vertexFile = fopen(vertexPath,"r");
    FILE* fragFile = fopen(fragPath, "r");
    if(!vertexFile){
        ERROR("Failed to open file %s, ERRNO: %s", vertexPath, strerror(errno));
    }
    if(!fragFile){
        ERROR("Failed to open file %s ERRNO: %s", fragPath, strerror(errno));
    }



    char buffer[MAX_BUFFER_SIZE];
    
    char* vertexString  = malloc(sizeof(char) * 1);
    vertexString[0] = '\0';
    char* fragString  = malloc(sizeof(char) * 1);
    fragString[0] = '\0';


    while(fscanf(vertexFile,"%s", buffer) != EOF){
        char* concat  = concact_strings(vertexString, buffer);
        free(vertexString);
        vertexString = concat;
    }

    while(fscanf(fragFile,"%s", buffer) != EOF){
        char* concat  = concact_strings(fragString, buffer);
        free(fragString);
        fragString = concat;
    }

    printf("%s\n", vertexString);
    free(vertexString);
    free(fragString);

    fclose(vertexFile);
    fclose(fragFile);

    return 0;

}

inline void destroyShader(Shader_id id){
    glDeleteProgram(id);
}

inline void useShader(Shader_id id){
    glUseProgram(id);
}
