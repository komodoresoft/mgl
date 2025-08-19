// Modern Legacy OpenGL - mgl
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Graphic Libraries
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define W_WIDTH  (800)
#define W_HEIGHT (600)

GLuint dl;

int main(int argc, char **argv)
{
  float angle = 0.0;
  
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "MGL Test", NULL, NULL);
  if (!window) {
    printf("error: glfwCreateWindow() failed.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  dl = glGenLists(1);
  
  glNewList(dl, GL_COMPILE);
  glBegin(GL_LINES);
    glVertex2f(-20.0, -20.0); glVertex2f( 20.0, -20.0);
    glVertex2f( 20.0, -20.0); glVertex2f(-20.0,  20.0);
    glVertex2f(-20.0,  20.0); glVertex2f(-20.0, -20.0);
  glEnd();
  glEndList();
  
  glEnable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, W_WIDTH, W_HEIGHT, 0);
  
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(W_WIDTH/2, W_HEIGHT/2, 0.0);
    
    glColor3f(1.0, 1.0, 1.0);
    glCallList(dl);
    
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(50.0, 0.0, 0.0);
    glCallList(dl);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16);
    
    angle += 1.0;
  }
  
  glDeleteLists(dl, 1);
  
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}