#include "headers/Shader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024


char* readFromFile(FILE *f);

void checkCompileErrors(GLuint shader, const char* type);


Shader_id loadShader(const char* vertexPath, const char* fragPath){

    Shader_id ID;

    errno = 0;
    FILE* vertexFile = fopen(vertexPath,"rb");
    FILE* fragFile = fopen(fragPath, "rb");
    if(!vertexFile){
        ERROR("Failed to open file %s", vertexPath);
    }
    if(!fragFile){
        ERROR("Failed to open file %s", fragPath);
    }

    char* vertexString = readFromFile(vertexFile);
    char* fragString = readFromFile(fragFile);

    fclose(vertexFile);
    fclose(fragFile);

    GLuint vertex, fragment;

    //VERTEX SHADER
    vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, &vertexString, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    //FRAGMENT SHADER
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragString, NULL);
    glCompileShader(fragment);
    checkCompileErrors(vertex, "FRAGMENT");

    // PROGRAM SHADER
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertexString);
    free(fragString);
 
    return ID;

}

inline void destroyShader(Shader_id id){
    glDeleteProgram(id);
}


char* readFromFile(FILE *f){
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    rewind(f);
    if (length == -1 || (unsigned long) length >= SIZE_MAX) {
        printf("hey\n");
        // return NULL;
    }

    // Convert from long to size_t
    size_t ulength = (size_t) length;
    char *result = malloc(ulength + 1);
    // Allocation failed? Read incomplete?
    if (result == NULL || fread(result, 1, ulength, f) != ulength) {
        free(result);
        printf("hey\n");
        // return NULL;
    }
    result[ulength] = '\0'; // Now buffer points to a string
    return result;
}

inline void useShader(Shader_id id){
    glUseProgram(id);
}


void checkCompileErrors(GLuint shader, const char* type)
{
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type:%s\n %s\n", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type:%s\n %s\n", type, infoLog);
        }
    }
}