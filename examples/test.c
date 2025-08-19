// Modern Legacy OpenGL - mgl
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Graphic Libraries
#include <MGL/mgl.h>
#include <MGL/mglu.h>
#include <GLFW/glfw3.h>

#define W_WIDTH  (800)
#define W_HEIGHT (600)

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
  
  dl = mglGenLists(1);
  
  glEnable(GL_DEPTH_TEST);
  
  mglMatrixMode(GL_PROJECTION);
  mglLoadIdentity();
  mgluOrtho2D(0, W_WIDTH, W_HEIGHT, 0);
  
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mglMatrixMode(GL_MODELVIEW);
    mglLoadIdentity();
    mglTranslatef(W_WIDTH/2, W_HEIGHT/2, 0.0);
    
    mglColor3f(1.0, 1.0, 1.0);
    mglBegin(GL_TRIANGLES);
      mglVertex2f(-20.0, -20.0); //mglVertex2f( 20.0, -20.0);
      mglVertex2f( 20.0, -20.0); //mglVertex2f(-20.0,  20.0);
      mglVertex2f(-20.0,  20.0); //mglVertex2f(-20.0, -20.0);
    mglEnd();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16);
    
    angle += 1.0;
  }
  
  mglDeleteLists(dl, 1);
  
  mglClose();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}