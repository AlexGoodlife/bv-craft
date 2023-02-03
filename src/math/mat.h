#ifndef BV_MAT_H
#define BV_MAT_H

#include <math.h>
#include <string.h> // mmcpy
#include "vec.h"

#include <stdio.h>

typedef struct mat2_s
{
    float m[2][2];
}mat2_s;

typedef struct mat3_s
{
    float m[3][3];
}mat3_s;

typedef struct mat4_s
{
    float m[4][4];
}mat4_s;

#define init_mat2_id (mat2_s){.m = {{1,0},{0,1}}}
#define init_mat3_id (mat3_s){.m = {{1,0,0},{0,1,0},{0,0,1}}}
#define init_mat4_id (mat4_s){.m ={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}}

#define init_mat2_zero {.m = {{0,0},{0,0}}}
#define init_mat3_zero {.m = {{0,0,0},{0,0,0},{0,0,0}}}
#define init_mat4_zero {.m ={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}}


#define mat4_debug_print(mat) do{for(int i = 0; i < 4;i++){printf("[ ");for(int j = 0; j < 4;j++){printf("%f ", mat.m[i][j]);}printf("]\n");}}while(0)


static inline mat4_s linealg_mat4cpy(mat4_s* base){
    mat4_s result;
    for(int i = 0; i < 4;i++)
        memcpy(result.m[i], base->m[i],sizeof(float) * 4);
    return result;
}


static inline mat4_s linealg_perspective(float aspectRatio, float fov, float near, float far){
    mat4_s result = init_mat4_zero;
    float fov_calc = tan(fov/2);
    float plane_calc = far/(far - near);

    result.m[0][0] = 1/(aspectRatio * fov_calc); 
    result.m[1][1] = 1/fov_calc;
    result.m[2][2] = plane_calc;
    result.m[3][2] = plane_calc * (-near);
    result.m[2][3] = 1;
    return result;
}

static inline mat4_s linealg_rotate(mat4_s* base, float angle, vec3_s* v){
    float cs = cos(angle);
    float sn = sin(angle);

    vec3_s axis = vec3_normalize(v);
    vec3_s temp = vec3((1-cs)*axis.x, (1-cs)*axis.y, (1-cs)*axis.z);
    // vec<3, T, Q> axis(normalize(v));
    // vec<3, T, Q> temp((T(1) - c) * axis);

    mat4_s rotate = init_mat4_zero;
    // mat<4, 4, T, Q> Rotate;
    rotate.m[0][0] = cs + temp.x * axis.x;
    rotate.m[0][1] = temp.x * axis.y + sn * axis.z;
    rotate.m[0][2] = temp.x * axis.z - sn * axis.y;

    rotate.m[1][0] = temp.y * axis.x - sn * axis.z;
    rotate.m[1][1] = cs + temp.y * axis.y;
    rotate.m[1][2] = temp.y * axis.z + sn * axis.x;

    rotate.m[2][0] = temp.z * axis.x + sn * axis.y;
    rotate.m[2][1] = temp.z * axis.y - sn * axis.x;
    rotate.m[2][2] = cs + temp.z * axis.z;

    mat4_s result = init_mat4_zero;
    
    for(int i = 0; i < 4;i++){
        result.m[0][i] = base->m[0][i] * rotate.m[0][0] + base->m[1][i] * rotate.m[0][1] + base->m[2][i] * rotate.m[0][2];
        result.m[1][i] = base->m[0][i] * rotate.m[1][0] + base->m[1][i] * rotate.m[1][1] + base->m[2][i] * rotate.m[1][2];
        result.m[2][i] = base->m[0][i] * rotate.m[2][0] + base->m[1][i] * rotate.m[2][1] + base->m[2][i] * rotate.m[2][2];
        result.m[3][i] = base->m[3][i];
    }
    return result;

}

static inline mat4_s linealg_translate(mat4_s* base, vec3_s* v){
    mat4_s result = linealg_mat4cpy(base);
    for(int i = 0; i < 4;i++){
        result.m[3][i] = base->m[0][i] * v->x + base->m[1][i] * v->y + base->m[2][i] * v->z + base->m[3][i];
    }
	return result;
}

static inline mat4_s linealg_lookat(vec3_s* eye, vec3_s* center, vec3_s* up){
    vec3_s forward = vec3_sub(center,eye);
    forward = vec3_normalize(&forward);

    vec3_s right = vec3_cross(up,&forward);


    right = vec3_normalize(&right);
    // vec3_negate(right);

    vec3_s newUp = vec3_cross(&forward,&right);

    mat4_s result = init_mat4_id;

    result.m[0][0] = right.x; result.m[1][0] = right.y; result.m[2][0] =  right.z;

    result.m[0][1] = newUp.x; result.m[1][1] = newUp.y; result.m[2][1] = newUp.z;

    result.m[0][2] = forward.x; result.m[1][2] = forward.y; result.m[2][2] = forward.z;

    result.m[3][0] = -vec3_dot(&right, eye);
    result.m[3][1] = -vec3_dot(&newUp, eye);
    result.m[3][2] = -vec3_dot(&forward, eye);

    return result;
}

static inline mat4_s linealg_scale(mat4_s* base, vec3_s* v){
    mat4_s result = init_mat2_zero;
    for(int i = 0; i < 4;i++){
        result.m[0][i] = base->m[0][i] * v->x;
        result.m[1][i] = base->m[1][i] * v->y;
        result.m[2][i] = base->m[2][i] * v->z;
        result.m[3][i] = base->m[3][i];
    }
    return result;
}

#endif