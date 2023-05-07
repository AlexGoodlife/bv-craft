#include "Shader.h"
#include <stdlib.h> // malloc
#include <errno.h>

#define MAX_BUFFER_SIZE 1024



char* readFromFile(FILE *f);

void checkCompileErrors(GLuint shader, const char* type);


Shader_id shader_load(const char* vertexPath, const char* fragPath){

    Shader_id ID;

    errno = 0;
    FILE* vertexFile = fopen(vertexPath,"rb");
    FILE* fragFile = fopen(fragPath, "rb");
    if(!vertexFile){
        ERROR("Failed to open file %s", vertexPath);
        fclose(vertexFile);
        fclose(fragFile);
        return 0xFEEDBEEF;
    }
    if(!fragFile){
        ERROR("Failed to open file %s", fragPath);
        fclose(vertexFile);
        fclose(fragFile);
        return 0xFEEDBEEF;
    }

    char* vertexString = readFromFile(vertexFile);
    char* fragString = readFromFile(fragFile);

    fclose(vertexFile);
    fclose(fragFile);

    GLuint vertex, fragment;

    //VERTEX SHADER
    vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, (const char**)&(vertexString), NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    //FRAGMENT SHADER
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&fragString, NULL);
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

void shader_setBool(Shader_id ID, const char* name, bool value){
    glUniform1i(glGetUniformLocation(ID, name), (int)value); 
}
void shader_setInt(Shader_id ID, const char* name, int value){
    glUniform1i(glGetUniformLocation(ID, name),value); 
}
void shader_setFloat(Shader_id ID, const char* name, float value){
    glUniform1f(glGetUniformLocation(ID, name),value); 
}
void shader_setVec2(Shader_id ID, const char* name, vec2_s* v){
    glUniform2fv(glGetUniformLocation(ID, name), 1,&(v->x)); 
}

void shader_setVec2l(Shader_id ID, const char* name, float x, float y){
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}   


void shader_setVec3(Shader_id ID, const char* name, vec3_s*v){
    glUniform3fv(glGetUniformLocation(ID, name),1,&(v->x)); 
}

void shader_setVec3l(Shader_id ID, const char* name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(ID, name), x, y,z);
}


void shader_setVec4(Shader_id ID, const char* name, vec4_s*v){
    glUniform4fv(glGetUniformLocation(ID, name),1,&(v->x)); 
}

void shader_setVec4l(Shader_id ID, const char* name, float x, float y, float z, float w){
    glUniform4f(glGetUniformLocation(ID, name), x, y,z,w);
}


void shader_setMat2(Shader_id ID, const char* name, mat2_s*m){
    glUniformMatrix2fv(glGetUniformLocation(ID, name),1,GL_FALSE,&(m->m[0][0])); 
}
void shader_setMat3(Shader_id ID, const char* name, mat3_s*m){
    glUniformMatrix3fv(glGetUniformLocation(ID, name),1,GL_FALSE,&(m->m[0][0])); 
}
void shader_setMat4(Shader_id ID, const char* name, mat4_s*m){
     glUniformMatrix4fv(glGetUniformLocation(ID, name),1,GL_FALSE,&(m->m[0][0])); 
}


void shader_destroy(Shader_id id){
    glDeleteProgram(id);
}


char* readFromFile(FILE *f){
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    rewind(f);
    if (length == -1 || (unsigned long) length >= SIZE_MAX) {
        return NULL;
    }

    // Convert from long to size_t
    size_t ulength = (size_t) length;
    char *result = malloc(ulength + 1);
    // Allocation failed? Read incomplete?
    if (result == NULL || fread(result, 1, ulength, f) != ulength) {
        free(result);
        return NULL;
    }
    result[ulength] = '\0'; // Now buffer points to a string
    return result;
}

void shader_use(Shader_id id){
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
            exit(-1);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type:%s\n %s\n", type, infoLog);
            exit(-1);
        }
    }
}
