
#include <stdio.h>
#include <stdbool.h>

#include "src/math/linealg.h"
#include "src/state.h"
#include "src/Block.h"
#include "src/Chunk.h"
#include "src/World.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define WORLD_WIDTH 2
#define WORLD_HEIGHT 2

void run(){


    // uint32_t test_map[CHUNK_DEPTH *CHUNK_HEIGHT*CHUNK_WIDTH];
    uint32_t* test_map = malloc(sizeof(uint32_t) * CHUNK_DEPTH * CHUNK_WIDTH * CHUNK_HEIGHT);
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                test_map[z*CHUNK_WIDTH*CHUNK_HEIGHT + y*CHUNK_WIDTH + x] = 3;
            }
        }
    }

    test_map[0+0+0] = 0;

    for(int i = 0; i < CHUNK_HEIGHT;i++){
        test_map[CHUNK_DEPTH/2 *CHUNK_WIDTH*CHUNK_HEIGHT + i*CHUNK_WIDTH + CHUNK_WIDTH/2] = 0;
    }

    test_map[CHUNK_DEPTH/2 *CHUNK_WIDTH*CHUNK_HEIGHT + 3*CHUNK_WIDTH + CHUNK_WIDTH/2-1] = 0;

    test_map[CHUNK_DEPTH/2 *CHUNK_WIDTH*CHUNK_HEIGHT + (CHUNK_HEIGHT-2)*CHUNK_WIDTH + CHUNK_WIDTH/2] = 1;

    GLuint Atlas_texture = loadTexture("resources/big_ass_atlas.png");
    Chunk* test = chunk_build(test_map);
    Chunk* test2 = chunk_build(test_map);
    Chunk* test3 = chunk_build(test_map);
    Chunk* test4 = chunk_build(test_map);

    Chunk* map[WORLD_HEIGHT* WORLD_WIDTH] = {
        test, test2,test3,test4
    };

    World* world = world_init(map, WORLD_WIDTH, WORLD_HEIGHT);

    free(test_map);


    shader_use(shader);
    shader_setInt(shader,"Atlas", 0);


    mat4_s projection = init_mat4_id;
    projection = linealg_perspective((float)state->windowWidth/(float)state->windowHeight,radians(state->camera->fov), 0.1f, 100.0f);
    while(!glfwWindowShouldClose(state->window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        displayFPS(deltaTime, lastFrame);

        processInput(deltaTime);

        glClearColor(BACKGROUND_COLOR_4C);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Atlas_texture);

        shader_use(shader);
        for(int y = 0; y < world->map_height;y++){
            for(int x = 0; x < world->map_width;x++){

                mat4_s view = init_mat4_id;
                mat4_s model = init_mat4_id;
                // model = linealg_rotate(&model, glfwGetTime(), &vec3(1.0f,1.0f,1.0f));
                model = linealg_scale(&model, &vec3(0.5f,0.5f,0.5f));
                model = linealg_translate(&model, &vec3(x*CHUNK_WIDTH,0.0f,y*CHUNK_DEPTH));

                view = camera_getViewMat(state->camera);

                // if(b){mat4_debug_print(view); b = false;};

                // view = linealg_translate(view, &vec3(0.0f,-1.0f,3.0f));

                // pass transformation matrices to the shader
                shader_setMat4(shader,"projection", &projection);
                shader_setMat4(shader,"model", &model);
                shader_setMat4(shader,"view", &view);

                chunk_render(world->chunk_map[INDEX2D(x,y,world->map_width)]);
            }
        }

        // chunk_render(test);
        // world_render(world);

        glfwPollEvents();
        glfwSwapBuffers(state->window);

    }

    world_destroy(world);
    // chunk_destroy(test);
    shader_destroy(shader);
    close();
}


int main(int argc, char**argv){
    if(!init("Testing", WINDOW_WIDTH, WINDOW_HEIGHT)) return -1;
    
    run();

    return 0;
}

