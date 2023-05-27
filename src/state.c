#include "state.h"
#include "Chunk.h"
#include "World.h"
#include "common.h"
#include "math/vec.h"
#include <GLFW/glfw3.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action,int mods);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);
void init_test_map();
State *state;
Shader_id shader;

FILE *logFile;

bool firstMouse = true;
float lastX;
float lastY;

int init(const char *windowTitle, int windowWidth, int windowHeight) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_SAMPLES, 16);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  state = malloc(sizeof(State));
  state->windowTitle = windowTitle;
  state->windowWidth = windowWidth;
  state->windowHeight = windowHeight;

  state->window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
  if (state->window == NULL) {
    fprintf(stderr, "Failed to create GLFW window");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(state->window);

#if !VSYNC
  glfwSwapInterval(0);
#endif
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD");
    return false;
  }

  glViewport(0, 0, windowWidth, windowHeight);

  int flags; 
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    printf("finna create\n");
  }

  glfwSetKeyCallback(state->window, key_callback);
  glfwSetFramebufferSizeCallback(state->window, framebuffer_size_callback);
  glfwSetCursorPosCallback(state->window, mouse_callback);
  glfwSetScrollCallback(state->window, scroll_callback);
  glfwSetMouseButtonCallback(state->window, mouse_button_callback);
  glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
 // glDepthMask(GL_FALSE);
  // glDisable(GL_MULTISAMPLE);

  shader = shader_load("src/shaders/tex_array.vert", "src/shaders/tex_array.frag");
  if (errno) {
    ERROR("FAILED TO LOAD FILE");
    return false;
  }

  state->camera = camera_init_new_default();

  //stbi_set_flip_vertically_on_load(true);

  lastX = (state->windowWidth) / 2.0;
  lastY = (state->windowHeight) / 2.0;

  blockmesh_buildAllBlocks();

  logFile = fopen("log.txt", "w");
  state->thread_pool = threadpool_init(N_THREADS); 
  init_test_map();
  worldgen_init();

  shader_use(shader);
  shader_setVec3l(shader, "dirlight.direction", 0 -0.2f, -1.0f, -0.3f);
  shader_setVec3l(shader, "dirlight.ambient", 0.4f, 0.4f,0.4f);
  shader_setVec3l(shader, "dirlight.diffuse",0.5f, 0.5f, 0.5f);
  shader_setVec3l(shader, "dirlight.specular",0.5f, 0.5f,0.5f);
  shader_setFloat(shader, "shininess", 32.0f);

  printf("I init\n");
  return true;
}

void close() {
  threadpool_destroy(&state->thread_pool);
  free(state->camera);
  free(state);
  fclose(logFile);
  free(all_blocks);
  glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

bool poly = false;

#define RAYCAST 1

void mouse_button_callback(GLFWwindow *window, int button, int action,int mods) {
#if RAYCAST
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    Raycast_Payload raycast = world_raycast(world, state->camera->pos, state->camera->front);
    world_break_block(world, raycast);
  }
  if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    Raycast_Payload raycast = world_raycast(world, state->camera->pos, state->camera->front);
    world_place_block(world, raycast);
  }
#endif
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    if (!poly) {
      poly = true;
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      poly = false;
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void processInput(float deltaTime) {
  if (glfwGetKey(state->window, GLFW_KEY_W) == GLFW_PRESS)
    camera_processKeyboard(state->camera, FORWARD, deltaTime);
  if (glfwGetKey(state->window, GLFW_KEY_S) == GLFW_PRESS)
    camera_processKeyboard(state->camera, BACKWARDS, deltaTime);
  if (glfwGetKey(state->window, GLFW_KEY_A) == GLFW_PRESS)
    camera_processKeyboard(state->camera, LEFT, deltaTime);
  if (glfwGetKey(state->window, GLFW_KEY_D) == GLFW_PRESS)
    camera_processKeyboard(state->camera, RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = (float)(xposIn);
  float ypos = (float)(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera_processMouse(state->camera, xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera_processScrollWheel(state->camera, yoffset);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    printf("---------------------\n");

    printf("Debug message (%d) : %s\n", id, message); 

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             printf("%s", "Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("%s", "Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("%s", "Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("%s", "Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     printf("%s", "Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           printf("%s", "Source: Other"); break;
    } printf("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               printf("%s", "Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("%s", "Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("%s", "Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         printf("%s", "Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         printf("%s", "Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              printf("%s", "Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          printf("%s", "Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           printf("%s", "Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               printf("%s", "Type: Other"); break;
    } printf("\n");    

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         printf("%s", "Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       printf("%s", "Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          printf("%s", "Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("%s", "Severity: notification"); break;
    } printf("\n");        
    printf("\n");
}

double waitTime = 0.0f;
void displayFPS(float deltaTime, float lastFrame) {
  // nbFrames++;

  if (lastFrame - waitTime >= 1.0) {

    char buff[254];
    sprintf(buff, "%s [%.4f ms]", state->windowTitle, deltaTime * 1000);

    glfwSetWindowTitle(state->window, buff);
    waitTime = lastFrame;
  }
}

void init_test_map(){

  test_map = malloc(sizeof(uint8_t) * CHUNK_DEPTH * CHUNK_WIDTH * CHUNK_HEIGHT);
  for (uint32_t z = 0; z < CHUNK_DEPTH; z++) {
    for (uint32_t y = 0; y < CHUNK_HEIGHT; y++) {
      for (uint32_t x = 0; x < CHUNK_WIDTH; x++) {
        if(y < CHUNK_HEIGHT/2)
          test_map[z * CHUNK_WIDTH * CHUNK_HEIGHT + y * CHUNK_WIDTH + x] = 3;
        else
          test_map[z * CHUNK_WIDTH * CHUNK_HEIGHT + y * CHUNK_WIDTH + x] = 0;
      }
    }
  }

  test_map[0 + 0 + 0] = 0;

  // for (int i = 0; i < CHUNK_HEIGHT; i++) {
  //   test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT + i * CHUNK_WIDTH +
  //            CHUNK_WIDTH / 2] = 0;
  // }
  //
  // test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT + 3 * CHUNK_WIDTH +
  //          CHUNK_WIDTH / 2 - 1] = 0;
  //
  // test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT +
  //          (CHUNK_HEIGHT - 2) * CHUNK_WIDTH + CHUNK_WIDTH / 2] = 1;
}

GLuint loadTexture(const char *path) {
  errno = 0;
  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);

    int base_mip_level = 0;
    int max_mip_level = 4; // for example, use up to the 4th mipmap level
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, base_mip_level);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max_mip_level);    
    glGenerateMipmap(GL_TEXTURE_2D);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float max_anisotropy = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
    printf("ANISOTROPY: %f\n", max_anisotropy);
    glCheckError();
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);

    stbi_image_free(data);
  } else {
    ERROR("Failed to load texture file %s: \n", path);
    stbi_image_free(data);
  }

  return textureID;
}

static void get_textures_from_atlas_data(GLuint ID, unsigned char* data, int n_textures, int atlas_x, int atlas_y, int texture_width, int texture_height, int channels, GLenum format){
  glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
  glCheckError();
  int tex_size_x = texture_width * channels;
  printf(" TEX_SIZE:X %d\n", tex_size_x);
  int row_len = atlas_x * tex_size_x;

  unsigned char* tile = calloc(texture_height * texture_width * channels, sizeof(unsigned char));
  //unsigned char* tile = malloc(sizeof(unsigned char) * texture_height * texture_width * channels);
  
  for(int iy = 0; iy < atlas_y;iy++){
   for(int ix = 0; ix < atlas_x;ix++){

      unsigned char* ptr = data + iy*row_len*texture_height + ix*tex_size_x;
      for(int row = 0; row < texture_height; row++){
        memcpy(tile + row*tex_size_x, ptr + row*row_len, tex_size_x*sizeof(unsigned char));
      }
      int i = iy * atlas_x + ix;
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, TEXTURE_WIDTH, TEXTURE_HEIGHT, 1, format, GL_UNSIGNED_BYTE, tile);
      glCheckError();
    }
  }
  free(tile);


}

GLuint loadTextureArray(const char* path){
  errno = 0;
  GLuint textureArrayID;
  glGenTextures(1, &textureArrayID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
    glCheckError();

    int n_textures = (ATLAS_WIDTH*ATLAS_HEIGHT)/(TEXTURE_HEIGHT* TEXTURE_WIDTH);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,format , TEXTURE_WIDTH, TEXTURE_HEIGHT, n_textures, 0, format, GL_UNSIGNED_BYTE, NULL);
    glCheckError();

    get_textures_from_atlas_data(textureArrayID, data, n_textures, ATLAS_WIDTH/TEXTURE_WIDTH, ATLAS_HEIGHT/TEXTURE_HEIGHT, TEXTURE_WIDTH, TEXTURE_HEIGHT, nrComponents,format);

    int base_mip_level = 0;
    int max_mip_level = 4; // for example, use up to the 4th mipmap level
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, base_mip_level);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, max_mip_level);    
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glCheckError();


    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glCheckError();

    float max_anisotropy = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
    printf("ANISOTROPY: %f\n", max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);

    stbi_image_free(data);
  } else {
    ERROR("Failed to load texture file %s: \n", path);
    stbi_image_free(data);
  }

  return textureArrayID;
}


