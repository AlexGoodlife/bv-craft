#ifndef BV_VEC_H
#define BV_VEC_H

#include <math.h>

typedef struct{
    float x, y;
}vec2_s;

typedef struct{
    float x, y,z;
}vec3_s;

typedef struct{
    float x, y,z,w;
}vec4_s;

#define vec2(x_,y_) {.x = x_, .y = y_}
#define vec3(x_,y_,z_) {.x = x_, .y = y_, .z = z_}
#define vec4(x_,y_,z_,w_) {.x = x_, .y = y_,.z = z_,.w = w_}

#define vec_dot(v,u) _Generic((v), vec2_s*: vec2_dot, vec3_s*: vec3_dot, vec4_s*: vec4_dot)(v,u)
#define vec_normalize(v) _Generic((v), vec2_s*: vec2_normalize, vec3_s*: vec3_normalize, vec4_s*: vec4_normalize)(v)

#define vec3_multiply_const(vec, c) ({vec.x *= c; vec.y *= c; vec.z *= c})

static inline float vec2_dot(vec2_s* v, vec2_s* u){
    return sqrtf(v->x * u->x + v->y * u->y);
}

static inline vec2_s vec2_normalize(vec2_s* v){
    vec2_s result = vec2(v->x, v->y);
    float dot = vec2_dot(v,v);
    result.x /= dot; 
    result.y /= dot;
    return result; 
}

static inline float vec3_dot(vec3_s* v, vec3_s* u){
    return sqrtf(v->x * u->x + v->y * u->y + v->z * u->z);
}

static inline vec3_s vec3_normalize(vec3_s* v){
    vec3_s result = vec3(v->x, v->y, v->z);
    float dot = vec3_dot(v,v);
    result.x /= dot; 
    result.y /= dot;
    result.z /= dot;
    return result; 
}
static inline float vec4_dot(vec4_s* v, vec4_s* u){
    return sqrtf(v->x * u->x + v->y * u->y + v->z * u->z + v->w*u->w);
}

static inline vec4_s vec4_normalize(vec4_s* v){
    vec4_s result = vec4(v->x, v->y, v->z, v->w);
    float dot = vec4_dot(v,v);
    result.x /= dot; 
    result.y /= dot;
    return result; 
}


// vec2_s vec2(float x, float y);

// vec3_s vec3(float x, float y, float z);

// vec4_s vec4(float x, float y, float z);


#endif