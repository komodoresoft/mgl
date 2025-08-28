// MGL mgluLookAt Example
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Graphic Libraries
#include <MGL/mgl.h>
#include <MGL/mglu.h>
#include <GLFW/glfw3.h>

#define W_WIDTH  (800)
#define W_HEIGHT (600)

#define DEG2RAD(a) ((a) * M_PI/180.0)

GLuint dl;

int main(int argc, char **argv)
{
  float angle = 0.0;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "MGL Test", NULL, NULL);
  if (!window) {
    printf("error: glfwCreateWindow() failed.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  mglInit((GLADloadproc)glfwGetProcAddress);
  
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mglMatrixMode(GL_PROJECTION);
    mglLoadIdentity();
    mgluPerspective(60.0, (float)W_WIDTH/(float)W_HEIGHT, 0.1, 100.0);
    mgluLookAt(
      sin(DEG2RAD(angle)) * 5.0, 0.0, cos(DEG2RAD(angle)) * 5.0,
      0.0, 0.0, 0.0,
      0.0, 1.0, 0.0
    );
    
    mglMatrixMode(GL_MODELVIEW);
    mglLoadIdentity();
    
    mglColor3f(1.0, 1.0, 1.0);
    mglBegin(GL_TRIANGLES);
      mglColor3f(1.0, 0.0, 0.0); mglVertex2f( 0.0,  1.0);
      mglColor3f(0.0, 1.0, 0.0); mglVertex2f(-1.0, -1.0);
      mglColor3f(0.0, 0.0, 1.0); mglVertex2f( 1.0, -1.0);
    mglEnd();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16);
    
    angle += 1.0;
  }
  
  mglClose();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}