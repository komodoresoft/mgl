// **********************************
// Example of a rotating texured-cube
// **********************************

#include <stdio.h>
#include <stdlib.h>

#include <MGL/mgl.h>
#include <MGL/mglu.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define W_WIDTH  (800)
#define W_HEIGHT (600)

typedef struct {
  int t;
  int v;
  int vt;
  int vn;
} FaceIndex;

float vertices[][3] = {
  { 1.0,  1.0, -1.0},
  { 1.0, -1.0, -1.0},
  { 1.0,  1.0,  1.0},
  { 1.0, -1.0,  1.0},
  {-1.0,  1.0, -1.0},
  {-1.0, -1.0, -1.0},
  {-1.0,  1.0,  1.0},
  {-1.0, -1.0,  1.0}
};

float texcoords[][2] = {
  {0.0, 1.0},
  {1.0, 0.0},
  {1.0, 1.0},
  {0.0, 0.0}
};

float normals[][3] = {
  { 0.0,  1.0,  0.0},
  { 0.0,  0.0,  1.0},
  {-1.0,  0.0,  0.0},
  { 0.0, -1.0,  0.0},
  { 1.0,  0.0,  0.0},
  { 0.0,  0.0, -1.0}
};

FaceIndex faces[][3] = {
  {{0, 3,2,2}, {0, 8,1,2}, {0, 4,3,2}}, // A
  {{0, 3,2,2}, {0, 7,4,2}, {0, 8,1,2}},
  {{1, 1,3,5}, {0, 4,4,5}, {0, 2,2,5}}, // B
  {{1, 1,3,5}, {0, 3,1,5}, {0, 4,4,5}},
  {{2, 2,3,4}, {0, 8,4,4}, {0, 6,1,4}}, // C
  {{2, 2,3,4}, {0, 4,2,4}, {0, 8,4,4}},
  {{3, 5,3,6}, {0, 2,4,6}, {0, 6,2,6}}, // D
  {{3, 5,3,6}, {0, 1,1,6}, {0, 2,4,6}},
  {{4, 5,1,1}, {0, 3,2,1}, {0, 1,3,1}}, // E
  {{4, 5,1,1}, {0, 7,4,1}, {0, 3,2,1}},
  {{5, 7,3,3}, {0, 6,4,3}, {0, 8,2,3}}, // F
  {{5, 7,3,3}, {0, 5,1,3}, {0, 6,4,3}},
};

GLuint tpage[6];
const char *tpage_files[] = {
  "images/A.png",
  "images/B.png",
  "images/C.png",
  "images/D.png",
  "images/E.png",
  "images/F.png"
};

GLuint load_tpage(const char *filename);
int load_Textures(void);
void unload_Textures(void);

int main(void)
{
  float angle = 0.0;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Blockmania", NULL, NULL);
  if (!window) {
    printf("error: glfwCreateWindow() failed.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  mglInit((GLADloadproc)glfwGetProcAddress);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  
  load_Textures();
  
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mglMatrixMode(GL_PROJECTION);
    mglLoadIdentity();
    mgluPerspective(60.0, (float)W_WIDTH/(float)W_HEIGHT, 0.1, 100.0);
    mglTranslatef(0.0, 0.0, -5.0);
    
    mglMatrixMode(GL_MODELVIEW);
    mglLoadIdentity();
    
    mglColor3f(1.0, 1.0, 1.0);
    mglRotatef(angle, 1.0, 1.0, 0.0);
    for (int i = 0; i < 12; ++i) {
      mglBindTexture(GL_TEXTURE_2D, tpage[faces[i][0].t]);
      mglBegin(GL_TRIANGLES);
      int v1 = faces[i][0].v-1;
      int v2 = faces[i][1].v-1;
      int v3 = faces[i][2].v-1;
      int vt1 = faces[i][0].vt-1;
      int vt2 = faces[i][1].vt-1;
      int vt3 = faces[i][2].vt-1;
      mglTexCoord2f(texcoords[vt1][0], texcoords[vt1][1]); mglVertex3f(vertices[v1][0], vertices[v1][1], vertices[v1][2]);
      mglTexCoord2f(texcoords[vt2][0], texcoords[vt2][1]); mglVertex3f(vertices[v2][0], vertices[v2][1], vertices[v2][2]);
      mglTexCoord2f(texcoords[vt3][0], texcoords[vt3][1]); mglVertex3f(vertices[v3][0], vertices[v3][1], vertices[v3][2]);
      mglEnd();
    }
  
    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16);
    
    angle += 1.0;
  }
  
  unload_Textures();
  
  mglClose();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

int load_Textures(void)
{
  for (int i = 0; i < 6; ++i) {
    tpage[i] = load_tpage(tpage_files[i]);
  }
  return 0;
}

void unload_Textures(void)
{
  for (int i = 0; i < 6; ++i) {
    glDeleteTextures(1, &tpage[i]);
  }
}

GLuint load_tpage(const char *filename)
{
  GLuint id = 0;
  
  int x, y, n;
  char *data = stbi_load(filename, &x, &y, &n, 4);
  if (!data) {
    printf("stbi_load failed loading %s\n", filename);
    return id;
  }
  
  glGenTextures(1, &id);
  mglBindTexture(GL_TEXTURE_2D, id);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);
  return id;
}