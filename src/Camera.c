#include "Camera.h"
#include <stdlib.h> // malloc


void updateCamera(Camera* camera);

mat4_s camera_getViewMat(Camera* camera){
    vec3_s vDir = vec3_add(&camera->pos, &camera->front);
    return linealg_lookat(&camera->pos,&vDir, &camera->up);
}

Camera camera_init(vec3_s pos,vec3_s front, vec3_s up, float yaw, float pitch, float fov, float sensitivity ,float speed){
    Camera result = {
        .pos = pos,
        .front = front,
        .worldUp = up,
        .yaw = yaw,
        .pitch = pitch,
        .fov = fov,
        .sensitivity = sensitivity,
        .speed = speed
    };
    updateCamera(&result);
    return result;

}

Camera* camera_init_new(vec3_s pos,vec3_s front, vec3_s up, float yaw, float pitch, float fov, float sensitivity ,float speed){
    Camera* result = malloc(sizeof(Camera));
    *result = camera_init(pos,front,up,yaw,pitch,fov,sensitivity,speed);
    return result;
}

void camera_processKeyboard(Camera* camera, enum CameraDirection direction, float deltaTime){
    float velocity = camera->speed * deltaTime;
    if (direction == FORWARD){
        vec3_s mult = vec3_cpy(camera->front);
        vec3_multiply_const(mult,velocity);
        camera->pos = vec3_add(&camera->pos, &mult);
    }
    if (direction == BACKWARDS){
        vec3_s mult = vec3_cpy(camera->front);
        vec3_multiply_const(mult,velocity);
        camera->pos = vec3_sub(&camera->pos, &mult);
    }
    if (direction == LEFT){
        vec3_s mult = vec3_cpy(camera->right);
        vec3_multiply_const(mult,velocity);
        camera->pos = vec3_sub(&camera->pos, &mult);
    }
    if (direction == RIGHT){
        vec3_s mult = vec3_cpy(camera->right);
        vec3_multiply_const(mult,velocity);
        camera->pos = vec3_add(&camera->pos, &mult);
    }
}

void camera_processMouse(Camera* camera, float xOffset, float yOffset, bool ConstrainPitch){
    xOffset *= camera->sensitivity;
    yOffset *= camera->sensitivity;

    camera->yaw += xOffset;
    camera->pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if(ConstrainPitch){
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
    updateCamera(camera);
}

void camera_processScrollWheel(Camera* camera, float yOffset){
    camera->fov -= (float)yOffset;
    if (camera->fov < 1.0f)
        camera->fov = 1.0f;
    if (camera->fov > 45.0f)
        camera->fov = 45.0f;
}

void updateCamera(Camera* camera){
    vec3_s front = vec3(0.0f,0.0f,0.0f);
    front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
    front.y = sin(radians(camera->pitch));
    front.z = -(sin(radians(camera->yaw)) * cos(radians(camera->pitch)));


    camera->front = vec3_normalize(&front);

    vec3_s temp_right = vec3_cross(&camera->worldUp,&camera->front);
    camera->right = vec3_normalize(&temp_right);
    // printf("%f %f %f", camera->right.x, camera->right.y,camera->right.z);
    vec3_s temp_up = vec3_cross(&camera->front,&camera->right);
    camera->up = vec3_normalize(&temp_up);

}