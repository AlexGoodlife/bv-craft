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


#define SIGN(x) ((x >= 0) - (x < 0))
#define ABS(x) (x * SIGN(x))


#define vec2(x_,y_) (vec2_s){.x = x_, .y = y_}
#define vec3(x_,y_,z_) (vec3_s){.x = x_, .y = y_, .z = z_}
#define vec4(x_,y_,z_,w_) (vec4_s){.x = x_, .y = y_,.z = z_,.w = w_}

#define vec2_cpy(vec) vec2(vec.x,vec.y)
#define vec3_cpy(vec) vec3(vec.x,vec.y,vec.z)
#define vec4_cpy(vec) vec4(vec.x,vec.y,vec.z,vec.w)

#define vec_dot(v,u) _Generic((v), vec2_s*: vec2_dot, vec3_s*: vec3_dot, vec4_s*: vec4_dot)(v,u)
#define vec_normalize(v) _Generic((v), vec2_s*: vec2_normalize, vec3_s*: vec3_normalize, vec4_s*: vec4_normalize)(v)

#define vec3_multiply_const(vec, c) ({vec.x *= c; vec.y *= c; vec.z *= c;})

#define vec3_negate(vec) ({vec.x = -vec.x; vec.y = -vec.y; vec.z = -vec.z;})

static inline vec2_s vec2_add(vec2_s* v, vec2_s *u){
    return vec2(v->x + u->x, v->y + u->y);
}
static inline vec3_s vec3_add(vec3_s* v, vec3_s *u){
    return vec3(v->x + u->x, v->y + u->y,v->z + u->z);
} 
static inline vec4_s vec4_add(vec4_s* v, vec4_s *u){
    return vec4(v->x + u->x, v->y + u->y, v->z + u->z, v->w + u->w);
}

static inline vec2_s vec2_sub(vec2_s* v, vec2_s *u){
    return vec2_add(v, &vec2(-(u->x), -(u->y)));
}
static inline vec3_s vec3_sub(vec3_s* v, vec3_s *u){
    return vec3_add(v, &vec3(-(u->x), -(u->y), -(u->z)));
} 
static inline vec4_s vec4_sub(vec4_s* v, vec4_s *u){
    return vec4_add(v, &vec4(-(u->x), -(u->y),-(u->z),-(u->w)));
}

static inline float vec2_dot(vec2_s* v, vec2_s* u){
    return v->x * u->x + v->y * u->y;
}

static inline vec2_s vec2_normalize(vec2_s* v){
    vec2_s result = vec2(v->x, v->y);
    float dot = vec2_dot(v,v);
    float inverse = sqrtf(ABS(dot));
    result.x /= inverse; 
    result.y /= inverse;
    return result; 
}

static inline float vec3_dot(vec3_s* v, vec3_s* u){
    return v->x * u->x + v->y * u->y + v->z * u->z;
}

static inline vec3_s vec3_normalize(vec3_s* v){
    vec3_s result = vec3(v->x, v->y, v->z);
    float dot = vec3_dot(v,v);
    float inverse = sqrtf(ABS(dot));
    result.x /= inverse; 
    result.y /= inverse;
    result.z /= inverse;
    return result; 
}
static inline float vec4_dot(vec4_s* v, vec4_s* u){
    return v->x * u->x + v->y * u->y + v->z * u->z + v->w*u->w;
}

static inline vec4_s vec4_normalize(vec4_s* v){
    vec4_s result = vec4(v->x, v->y, v->z, v->w);
    float dot = vec4_dot(v,v);
    float inverse = sqrtf(ABS(dot));
    result.x /= inverse; 
    result.y /= inverse;
    result.z /= inverse;
    result.w /= inverse;
    return result; 
}

static inline vec3_s vec3_cross(vec3_s *v, vec3_s *u){
    return vec3(v->y*u->z - v->z*u->y, v->z*u->x - v->x*u->z, v->x*u->y - v->y*u->x); 
}



#endif