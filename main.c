
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

#include "src/headers/Shader.h"

#define BACKGROUND_COLOR_R 0.2f
#define BACKGROUND_COLOR_G 0.3f
#define BACKGROUND_COLOR_B 0.3f
#define BACKGROUND_COLOR_A 1.0f
#define BACKGROUND_COLOR_4C BACKGROUND_COLOR_R, BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,BACKGROUND_COLOR_A 

int init();
void run();
void processInput();


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void displayFPS();

GLFWwindow* window;

const char* windowTitle = "Testing";
const int windowWidth = 800;
const int windowHeight = 600;

Shader_id shader;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
uint32_t nbFrames = 0;

void run(){

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    uint32_t indices[] = {
        0,1,3,
        1,2,3
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        displayFPS();
        processInput();
        glClearColor(BACKGROUND_COLOR_4C);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(shader);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_destroy(shader);
    glfwTerminate();
}


int init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if(window == NULL){
        fprintf(stderr,"Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,"Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    // glfwSetCursorPosCallback(window, Engine::mouse_callback);
    // glfwSetScrollCallback(window, Engine::scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    glEnable(GL_DEPTH_TEST);

    shader = shader_load("src/shaders/basic.vertex","src/shaders/basic.frag");
    if (errno)
    {
        ERROR("FAILED TO LOAD FILE");
        return false;
    }
    

    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

double waitTime;
void displayFPS(){
     
    nbFrames++;

    if(lastFrame - waitTime >= 1.0){
        double fps = (double)nbFrames;
        char buff[254];

        sprintf(buff, "%s [%lf FPS]", windowTitle, fps);

        glfwSetWindowTitle(window,buff);

        nbFrames = 0;
        waitTime = lastFrame;
    }

}


int main(int argc, char**argv){
    if(!init()) return -1;
    
    run();

    return 0;
}

