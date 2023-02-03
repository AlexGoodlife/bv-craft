
#include <stdio.h>
#include <stdbool.h>

#include "src/math/linealg.h"
#include "src/state.h"
#include "src/Block.h"
#include "src/Chunk.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void run(){

    // BlockMesh block = blockmesh_build(7);

    uint32_t test_map[CHUNK_DEPTH][CHUNK_HEIGHT*CHUNK_WIDTH];
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                test_map[z][y*CHUNK_WIDTH + x] = 1;
            }
        }
    }

    

    // GLuint VAO, VBO, EBO;

    // float vertices[block.vertexCount];
    // // uint32_t indices[block.indicesCount];

    // blockmesh_copyVertexData(&block, vertices);
    // // blockmesh_copyIndicesData(&block, indices);

    GLuint Atlas_texture = loadTexture("resources/atlas_mine.png");

    // glGenVertexArrays(1,&VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    // glBindVertexArray(VAO);


    // // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
    // glEnableVertexAttribArray(1);
    Chunk* test = chunk_build(test_map);

    // for(int i = 0; i < (4096* N_FACES)/8;i +=5){
    //     printf("%f %f %f %f %f\n", test->mesh->vertices[i],test->mesh->vertices[i+1],test->mesh->vertices[i+2],test->mesh->vertices[i+3],test->mesh->vertices[i+4]);
    // }

    // bool b = true;
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
        mat4_s view = init_mat4_id;
        mat4_s model = init_mat4_id;
        // model = linealg_rotate(&model, glfwGetTime(), &vec3(1.0f,1.0f,1.0f));
        model = linealg_scale(&model, &vec3(0.5f,0.5f,0.5f));
        // model = linealg_translate(model, &vec3(i,0.0f,0.0f));

        view = camera_getViewMat(state->camera);

        // if(b){mat4_debug_print(view); b = false;};

        // view = linealg_translate(view, &vec3(0.0f,-1.0f,3.0f));

        // pass transformation matrices to the shader
        shader_setMat4(shader,"projection", &projection);
        shader_setMat4(shader,"model", &model);
        shader_setMat4(shader,"view", &view);

        chunk_render(test);

        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT,0);


        glfwSwapBuffers(state->window);
        glfwPollEvents();

    }

    free(state->camera);
    free(state);
    chunk_destroy(test);
    free(all_blocks);
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    shader_destroy(shader);
    glfwTerminate();
}


int main(int argc, char**argv){
    if(!init("Testing", WINDOW_WIDTH, WINDOW_HEIGHT)) return -1;
    
    run();

    return 0;
}

