#ifndef BV_CAMERA_H
#define BV_CAMERA_H

#include "math/linealg.h"
#include <stdbool.h>
#include <stdlib.h> //free

enum CameraDirection{
    FORWARD,
    BACKWARDS,
    LEFT,
    RIGHT
};

const static float DEF_YAW         = -90.0f;
const static float DEF_PITCH       =  0.0f;
const static float DEF_SPEED       =  4.5f;
const static float DEF_SENSITIVITY =  0.1f;
const static float DEF_FOV        =  45.0f;


typedef struct{
    vec3_s pos;
    vec3_s front;
    vec3_s up;
    vec3_s right;
    vec3_s worldUp;

    float yaw;
    float pitch;
    float fov;
    float sensitivity;
    float speed;
}Camera;


mat4_s camera_getViewMat(Camera* camera);

Camera camera_init(vec3_s pos,vec3_s front, vec3_s up, float yaw, float pitch, float fov, float sensitivity ,float speed);

Camera* camera_init_new(vec3_s pos,vec3_s front, vec3_s up, float yaw, float pitch, float fov, float sensitivity ,float speed);

#define camera_init_default() camera_init(vec3(0.0f,0.0f,-3.0f), vec3(0.0f,0.0f,1.0f), vec3(0.0f,1.0f,0.0f), DEF_YAW, DEF_PITCH, DEF_FOV, DEF_SENSITIVITY, DEF_SPEED)
#define camera_init_new_default() camera_init_new(vec3(0.0f,0.0f,-3.0f), vec3(0.0f,0.0f,1.0f), vec3(0.0f,1.0f,0.0f), DEF_YAW, DEF_PITCH, DEF_FOV, DEF_SENSITIVITY, DEF_SPEED)


void camera_processKeyboard(Camera* camera, enum CameraDirection direction, float deltaTime);

void camera_processMouse(Camera* camera, float xOffset, float yOffset, bool ConstrainPitch);

void camera_processScrollWheel(Camera* camera, float yOffset);

#endif