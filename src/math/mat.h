#ifndef BV_MAT_H
#define BV_MAT_H

#include <math.h>
#include "vec.h"

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

#define init_mat2_id {.m = {{1,0},{0,1}}}
#define init_mat3_id {.m = {{1,0,0},{0,1,0},{0,0,1}}}
#define init_mat4_id {.m ={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}}

#define init_mat2_zero {.m = {{0,0},{0,0}}}
#define init_mat3_zero {.m = {{0,0,0},{0,0,0},{0,0,0}}}
#define init_mat4_zero {.m ={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}}

static inline mat4_s linealg_perspective(float aspectRatio, float fov, float near, float far){
    mat4_s result = init_mat4_zero;
    float fov_calc = (1/tan(fov/2));
    float plane_calc = far/(far - near);

    result.m[0][1] = aspectRatio * fov_calc; 
    result.m[1][1] = fov_calc;
    result.m[2][2] = plane_calc;
    result.m[3][2] = plane_calc * (-near);
    result.m[2][3] = 1;
    return result;
}

static inline mat4_s linealg_rotate(mat4_s base, float angle, vec3_s* v){
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
    
    // mat<4, 4, T, Q> result;
    for(int i = 0; i < 4;i++){
        result.m[0][i] = base.m[0][i] * rotate.m[0][0] + base.m[1][i] * rotate.m[0][1] + base.m[2][i] * rotate.m[0][2];
        result.m[1][i] = base.m[0][i] * rotate.m[1][0] + base.m[1][i] * rotate.m[1][1] + base.m[2][i] * rotate.m[1][2];
        result.m[2][i] = base.m[0][i] * rotate.m[2][0] + base.m[1][i] * rotate.m[2][1] + base.m[2][i] * rotate.m[2][2];
        result.m[3][i] = base.m[3][i];
    }
    // result.m[0] = base.m[0] * rotate.m[0][0] + base.m[1] * rotate.m[0][1] + base.m[2] * rotate.m[0][2];
    // result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    // result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    // result[3] = m[3];
    // return result;

    // Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
	// 	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	// 	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	// 	Result[3] = m[3];
}


#endif