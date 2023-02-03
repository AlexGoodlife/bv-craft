#include "state.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


State* state;
Shader_id shader;

bool firstMouse = true;
float lastX;
float lastY;

int init(const char* windowTitle, int windowWidth, int windowHeight){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    state = malloc(sizeof(State));
    state->windowTitle = windowTitle;
    state->windowWidth = windowWidth;
    state->windowHeight = windowHeight;

    state->window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if(state->window == NULL){
        fprintf(stderr,"Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(state->window);

    #ifndef VSYNC
    glfwSwapInterval(0);
    #endif
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,"Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(state->window, framebuffer_size_callback); 
    glfwSetCursorPosCallback(state->window, mouse_callback);
    glfwSetScrollCallback(state->window, scroll_callback);
    glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    glEnable(GL_DEPTH_TEST);

    shader = shader_load("src/shaders/textured.vert","src/shaders/textured.frag");
    if (errno)
    {
        ERROR("FAILED TO LOAD FILE");
        return false;
    }
    
    state->camera = camera_init_new_default();

    stbi_set_flip_vertically_on_load(true);

    lastX = (state->windowWidth) / 2.0;
    lastY =  (state->windowHeight) / 2.0;

    blockmesh_buildAllBlocks();

    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(float deltaTime){
    if(glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(state->window, true);
    if (glfwGetKey(state->window, GLFW_KEY_W) == GLFW_PRESS)
       camera_processKeyboard(state->camera, FORWARD,deltaTime);
    if (glfwGetKey(state->window, GLFW_KEY_S) == GLFW_PRESS)
        camera_processKeyboard(state->camera, BACKWARDS,deltaTime);
    if (glfwGetKey(state->window, GLFW_KEY_A) == GLFW_PRESS)
        camera_processKeyboard(state->camera, LEFT,deltaTime);
    if (glfwGetKey(state->window, GLFW_KEY_D) == GLFW_PRESS)
        camera_processKeyboard(state->camera, RIGHT,deltaTime);
}



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

    camera_processMouse(state->camera, xoffset,yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_processScrollWheel(state->camera, yoffset);
}



double waitTime = 0.0f;
void displayFPS(float deltaTime, float lastFrame){
    // nbFrames++;

    if(lastFrame - waitTime >= 1.0){
        // double fps = (double)nbFrames;
        // char buff[254];

        // sprintf(buff, "%s [%lf FPS]", state->windowTitle, fps);

        // glfwSetWindowTitle(state->window,buff);
        char buff[254];
        sprintf(buff, "%s [%lf ms]", state->windowTitle, deltaTime);

        glfwSetWindowTitle(state->window,buff);
        // nbFrames = 0;
        waitTime = lastFrame;
    }

}


GLuint loadTexture(const char* path){
    errno = 0;
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        ERROR("Failed to load texture file %s: \n", path);
        stbi_image_free(data);
    }

    return textureID;
}