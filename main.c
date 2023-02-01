
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

#include "src/Shader.h"
#include "src/Camera.h"
#include "src/math/linealg.h"
#include "src/Block.h"

#define BACKGROUND_COLOR_R 0.2f
#define BACKGROUND_COLOR_G 0.3f
#define BACKGROUND_COLOR_B 0.3f
#define BACKGROUND_COLOR_A 1.0f
#define BACKGROUND_COLOR_4C BACKGROUND_COLOR_R, BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,BACKGROUND_COLOR_A 

#define VSYNC

int init();
void run();
void processInput();


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void displayFPS();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

//STATE, WRAP UP LATER

GLFWwindow* window;
Camera* camera;


const char* windowTitle = "Testing";
const int windowWidth = 800;
const int windowHeight = 600;

Shader_id shader;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
uint32_t nbFrames = 0;

//END OF STATE

void run(){

    // float vertices[] = {
    //     //FRONT FACE
    //      0.5f,  0.5f, 0.0f,  // top right
    //      0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  // bottom left
    //     -0.5f,  0.5f, 0.0f,   // top left

    //     //BACK FACE
    //     //  0.5f,  0.5f, 0.5f,  // top right
    //     //  0.5f, -0.5f, 0.5f,  // bottom right
    //     // -0.5f, -0.5f, 0.5f,  // bottom left
    //     // -0.5f,  0.5f, 0.5f,   // top left 
    // };

    // float vertices[] = {
    //     -0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f, 
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f,  0.5f, -0.5f,  
    //     -0.5f,  0.5f, -0.5f, 
    //     -0.5f, -0.5f, -0.5f,  

    //     -0.5f, -0.5f,  0.5f,  
    //      0.5f, -0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //     -0.5f,  0.5f,  0.5f,  
    //     -0.5f, -0.5f,  0.5f,  

    //     -0.5f,  0.5f,  0.5f,  
    //     -0.5f,  0.5f, -0.5f,  
    //     -0.5f, -0.5f, -0.5f,  
    //     -0.5f, -0.5f, -0.5f,  
    //     -0.5f, -0.5f,  0.5f,  
    //     -0.5f,  0.5f,  0.5f, 

    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f,  0.5f, 
    //      0.5f,  0.5f,  0.5f, 

    //     -0.5f, -0.5f, -0.5f, 
    //      0.5f, -0.5f, -0.5f,  
    //      0.5f, -0.5f,  0.5f,  
    //      0.5f, -0.5f,  0.5f, 
    //     -0.5f, -0.5f,  0.5f,  
    //     -0.5f, -0.5f, -0.5f,  

    //     -0.5f,  0.5f, -0.5f, 
    //      0.5f,  0.5f, -0.5f,  
    //      0.5f,  0.5f,  0.5f,  
    //      0.5f,  0.5f,  0.5f, 
    //     -0.5f,  0.5f,  0.5f,  
    //     -0.5f,  0.5f, -0.5f, 
    // };

    // for(int i = 1; i < 108;i+=3){
    //     vertices[i] += 2.0f;
    // }

    // uint32_t indices[] = {
    //     0,1,3, // FRONT1
    //     1,2,3, // FRONT2

    //     // 4,5,7, // BACK1
    //     // 5,6,7, // BACK2

    //     // 5,6,0 // RIGHT2    

    // };


    BlockMesh block = blockmesh_build();

    GLuint VAO, VBO, EBO;

    float vertices[N_FACES * VERTEXES_PER_FACE * 3*2];
    uint32_t indices[FACE_INDICES_COUNT * N_FACES*2];

    for(int i = 0; i < FaceOrder_End;i++){
        memcpy(vertices + (i*3 * VERTEXES_PER_FACE), block.faces[i].vertexData, sizeof(float) * 3 * VERTEXES_PER_FACE);
        memcpy(indices + (i*6), block.faces[i].indicesData, sizeof(uint32_t) * 6);
    }
    for(int i = 0; i < FaceOrder_End;i++){
        for(int j = 0; j < 4;j++){
            block.faces[i].vertexData[j] = vec3_add(&block.faces[i].vertexData[j], &vec3(2.0f,0.0f,0.0f));
        }
        for(int k = 0; k < 6;k++){
            block.faces[i].indicesData[k] += 24;
        }
        memcpy(vertices + 72 + (i*3 * VERTEXES_PER_FACE), block.faces[i].vertexData, sizeof(float) * 3 * VERTEXES_PER_FACE);
        memcpy(indices + 36 + (i*6), block.faces[i].indicesData, sizeof(uint32_t) * 6);
    }


    // for(int i = 0; i < 72*2; i+=3){
    //     printf("%f %f %f\n", vertices[i], vertices[i+1], vertices[i+2]);
    // }


    // for(int i = 0; i < (72) ;i+=6){
    //     for(int j = i; j < i+6;j++){
    //         printf("%d ", indices[j]);
    //     }
    //     printf("\n");
    // }

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // bool b = true;

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        displayFPS();
        processInput();
        glClearColor(BACKGROUND_COLOR_4C);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(shader);



        mat4_s view = init_mat4_id;
        mat4_s projection = init_mat4_id;
        mat4_s model = init_mat4_id;
        model = linealg_rotate(model, glfwGetTime(), &vec3(1.0f,1.0f,0.0f));
        // model = linealg_translate(model, &vec3(i,0.0f,0.0f));

        view = camera_getViewMat(camera);

        // if(b){mat4_debug_print(view); b = false;};

        // view = linealg_translate(view, &vec3(0.0f,0.0f,6.0f));



        projection = linealg_perspective((float)windowWidth/(float)windowHeight,radians(camera->fov), 0.1f, 100.0f);



        // pass transformation matrices to the shader
        shader_setMat4(shader,"projection", &projection);
        shader_setMat4(shader,"model", &model);
        shader_setMat4(shader,"view", &view);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, 36*2,GL_UNSIGNED_INT,0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    free(camera);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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

    #ifndef VSYNC
    glfwSwapInterval(0);
    #endif
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,"Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    glEnable(GL_DEPTH_TEST);

    shader = shader_load("src/shaders/basic.vertex","src/shaders/basic.frag");
    if (errno)
    {
        ERROR("FAILED TO LOAD FILE");
        return false;
    }
    
    camera = camera_init_new_default();

    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       camera_processKeyboard(camera, FORWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_processKeyboard(camera, BACKWARDS,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_processKeyboard(camera, LEFT,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_processKeyboard(camera, RIGHT,deltaTime);
}

bool firstMouse = true;
float lastX =  windowWidth / 2.0;
float lastY =  windowHeight / 2.0;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)(xposIn);
    float ypos = (float)(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera_processMouse(camera, xoffset,yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_processScrollWheel(camera, yoffset);
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

