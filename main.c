
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>


#define BACKGROUND_COLOR_R 0.2f
#define BACKGROUND_COLOR_G 0.3f
#define BACKGROUND_COLOR_B 0.3f
#define BACKGROUND_COLOR_A 1.0f
#define BACKGROUND_COLOR_4C BACKGROUND_COLOR_R, BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,BACKGROUND_COLOR_A 

int init();
void run();
void processInput();


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* window;

const char* windowTitle = "Testing";
const int windowWidth = 800;
const int windowHeight = 600;


void run(){

    while(!glfwWindowShouldClose(window)){
        processInput();

        glClearColor(BACKGROUND_COLOR_4C);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

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
    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(int argc, char**argv){
    if(!init()) return -1;
    
    run();

    return 0;
}

