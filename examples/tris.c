// MGL 2x3 Rotating Triangles Example
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <MGL/mgl.h>
#include <MGL/mglu.h>
#include <GLFW/glfw3.h>

#define WIDTH  800
#define HEIGHT 600

typedef struct {
  float x, y;
  float angle;
  float d_a;
} Triangle;

float dir[] = {
  -1.0, 1.0,
  -1.0, 1.0,
  -1.0, 1.0,
};

int main() {
  srand(time(NULL));
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MGL 2x3 Triangles", NULL, NULL);
  if (!window) { glfwTerminate(); return -1; }
  glfwMakeContextCurrent(window);
  mglInit((GLADloadproc)glfwGetProcAddress);

  glEnable(GL_DEPTH_TEST);

  // Set up 2D orthographic screen
  mglMatrixMode(GL_PROJECTION);
  mglLoadIdentity();
  mgluOrtho2D(0, WIDTH, HEIGHT, 0);

  Triangle tris[6];
  int i, row, col;
  float spacingX = WIDTH / 3.0f;
  float spacingY = HEIGHT / 2.0f;

  // Initialize triangle positions
  for (i = 0; i < 6; i++) {
    row = i / 3;
    col = i % 3;
    tris[i].x = spacingX * (col + 0.5f);
    tris[i].y = spacingY * (row + 0.5f);
    tris[i].angle = 0.0f;
    tris[i].d_a = dir[rand()%6];
  }

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (i = 0; i < 6; i++) {
      mglMatrixMode(GL_MODELVIEW);
      mglLoadIdentity();
      mglTranslatef(tris[i].x, tris[i].y, 0.0f);
      mglRotatef(tris[i].angle, 0.0f, 0.0f, 1.0f);
      
      mglBegin(GL_TRIANGLES);
        mglColor3f(1.0f, 0.0f, 0.0f); mglVertex2f( 0.0f, -90.0f);
        mglColor3f(0.0f, 1.0f, 0.0f); mglVertex2f(-75.0f, 45.0f);
        mglColor3f(0.0f, 0.0f, 1.0f); mglVertex2f( 75.0f, 45.0f);
      mglEnd();

      tris[i].angle += tris[i].d_a;
      if (tris[i].angle >= 360.0f) tris[i].angle -= 360.0f;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16); // ~60 FPS
  }

  mglClose();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
