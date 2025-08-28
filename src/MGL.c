#include <MGL/mgl.h>

static const char* vshaderSource =
  "#version 330 core\n"
  "layout(location = 0) in vec3 aPos;\n"
  "layout(location = 1) in vec4 aColor;\n"
  "layout(location = 2) in vec2 aTexCoord;\n"
  "layout(location = 3) in vec3 aNormal;\n"
  "\n"
  "uniform mat4 u_ModelView;\n"
  "uniform mat4 u_Projection;\n"
  "\n"
  "out vec4 Color;\n"
  "out vec2 TexCoord;\n"
  "out vec3 Normal;\n"
  "\n"
  "void main() {\n"
  "  gl_Position = u_Projection * u_ModelView * vec4(aPos, 1.0);\n"
  "  Color = aColor;\n"
  "  TexCoord = aTexCoord;\n"
  "  Normal = aNormal;\n"
  "}\n"
;

static const char* fshaderSource =
  "#version 330 core\n"
  "in vec4 Color;\n"
  "in vec2 TexCoord;\n"
  "in vec3 Normal;\n"
  "\n"
  "uniform sampler2D u_Tex0;\n"
  "uniform bool u_UseTex;\n"
  "\n"
  "out vec4 FragColor;\n"
  "\n"
  "void main() {\n"
  "  if (u_UseTex) {\n"
  "    FragColor = texture(u_Tex0, TexCoord) * Color;\n"
  "  } else {\n"
  "    FragColor = Color;\n"
  "  }\n"
  "}\n"
;

GLuint __mgl_CurrentTexture;
int __mgl_VertexCount;
MGLVertex __mgl_VertexBuffer[MGL_MAX_VERTICES];
float __mgl_CurrentColor[4];
float __mgl_CurrentUV[2];
float __mgl_CurrentNormals[3];
GLuint __mgl_program;
GLuint __mgl_VBO, __mgl_VAO;
GLenum __mgl_DrawMode;
GLenum __mgl_MatrixMode;
MGLMatrix4x4 __mgl_ModelViewMatrix;
MGLMatrix4x4 __mgl_ProjectionMatrix;
MGLMatrix4x4 *__mgl_CurrentMatrix = NULL;
GLboolean __mgl_UseTex = GL_FALSE;
GLint __mgl_ModelViewUni;
GLint __mgl_ProjectionUni;
GLint __mgl_Tex0Uni;
GLint __mgl_UseTexUni;
MGLDisplayList *__mgl_DisplayLists[MAX_DISPLAYLISTS];
MGLDisplayList *__mgl_CurrentDisplayList = NULL;

MGLAPI int mglInit(GLADloadproc load_proc)
{
  GLuint __mgl_vshader;
  GLuint __mgl_fshader;
  
  if (!gladLoadGLLoader(load_proc)) {
    return -1;
  }
  
  // INIT GLOBALS
  __mgl_VertexCount = 0;
  memset(__mgl_CurrentColor, '\0', sizeof(__mgl_CurrentColor));
  memset(__mgl_CurrentUV, '\0', sizeof(__mgl_CurrentUV));
  memset(__mgl_CurrentNormals, '\0', sizeof(__mgl_CurrentNormals));
  __mgl_CurrentTexture = 0;
  
  // INIT MATRICES
  __mgl_ModelViewMatrix = mat4x4_identity();
  __mgl_ProjectionMatrix = mat4x4_identity();
  __mgl_CurrentMatrix = &__mgl_ModelViewMatrix;
  
  // INIT DISPLAY LIST
  for (int i = 0; i < MAX_DISPLAYLISTS; ++i)
    __mgl_DisplayLists[i] = NULL;
  
  // INIT SHADERS
  __mgl_vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(__mgl_vshader, 1, &vshaderSource, NULL);
  glCompileShader(__mgl_vshader);
  
  __mgl_fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(__mgl_fshader, 1, &fshaderSource, NULL);
  glCompileShader(__mgl_fshader);
  
  __mgl_program = glCreateProgram();
  glAttachShader(__mgl_program, __mgl_vshader);
  glAttachShader(__mgl_program, __mgl_fshader);
  glLinkProgram(__mgl_program);
  
  glDeleteShader(__mgl_vshader);
  glDeleteShader(__mgl_fshader);
  
  // INIT BUFFERS
  glGenVertexArrays(1, &__mgl_VAO);
  glBindVertexArray(__mgl_VAO);
  
  glGenBuffers(1, &__mgl_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, __mgl_VBO);
  glBufferData(GL_ARRAY_BUFFER, MGL_MAX_VERTICES * sizeof(MGLVertex), NULL, GL_DYNAMIC_DRAW);
  
  __mgl_ModelViewUni = glGetUniformLocation(__mgl_program, "u_ModelView");
  __mgl_ProjectionUni = glGetUniformLocation(__mgl_program, "u_Projection");
  __mgl_Tex0Uni = glGetUniformLocation(__mgl_program, "u_Tex0");
  __mgl_UseTexUni = glGetUniformLocation(__mgl_program, "u_UseTex");
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MGLVertex), (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MGLVertex), (void*)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MGLVertex), (void*)(7 * sizeof(float)));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MGLVertex), (void*)(9 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  return 0;
}
MGLAPI void mglClose(void)
{
  glDeleteVertexArrays(1, &__mgl_VAO);
  glDeleteBuffers(1, &__mgl_VBO);
  glDeleteProgram(__mgl_program);
}

MGLAPI void mglBegin(GLenum mode)
{
  if (__mgl_CurrentDisplayList) {
    __mgl_CurrentDisplayList->drawmode = mode;
    __mgl_CurrentDisplayList->count = 0;
  } else {
    __mgl_DrawMode = mode;
    __mgl_VertexCount = 0;
  }
}

MGLAPI void mglEnd(void)
{
  if (__mgl_CurrentDisplayList)
    return; // * yeah mglEnd() is useless with display lists
  
  glUseProgram(__mgl_program);
  glBindVertexArray(__mgl_VAO);
  
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(__mgl_Tex0Uni, 0);
  glUniform1i(__mgl_UseTexUni, __mgl_UseTex);
  
  glUniformMatrix4fv(__mgl_ModelViewUni, 1, GL_TRUE, &__mgl_ModelViewMatrix.m[0][0]);
  glUniformMatrix4fv(__mgl_ProjectionUni, 1, GL_TRUE, &__mgl_ProjectionMatrix.m[0][0]);
  
  glBindBuffer(GL_ARRAY_BUFFER, __mgl_VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, __mgl_VertexCount * sizeof(MGLVertex), __mgl_VertexBuffer);
  glDrawArrays(__mgl_DrawMode, 0, __mgl_VertexCount);
}

MGLAPI void mglBindTexture(GLenum target,GLuint texture)
{
  glBindTexture(target, texture);
  __mgl_CurrentTexture = texture;
  if (texture != 0)
    __mgl_UseTex = GL_TRUE;
  else
    __mgl_UseTex = GL_FALSE;
}

MGLAPI void mglColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
  __mgl_CurrentColor[0] = red;
  __mgl_CurrentColor[1] = green;
  __mgl_CurrentColor[2] = blue;
  __mgl_CurrentColor[3] = alpha;
}

MGLAPI void mglColor3f(GLfloat red,GLfloat green,GLfloat blue)
{
  mglColor4f(red, green, blue, 1.0);
}

MGLAPI void mglTexCoord2f(GLfloat s,GLfloat t)
{
  __mgl_CurrentUV[0] = s;
  __mgl_CurrentUV[1] = t;
}

MGLAPI void mglVertex3f(GLfloat x,GLfloat y,GLfloat z)
{ 
  MGLVertex *v = NULL;
  if (__mgl_CurrentDisplayList) {
    if (__mgl_CurrentDisplayList->count >= MGL_MAX_VERTICES)
      return;
    
    v = &__mgl_CurrentDisplayList->vtxbuffer[__mgl_CurrentDisplayList->count];
    __mgl_CurrentDisplayList->count++;
  } else {
    if (__mgl_VertexCount >= MGL_MAX_VERTICES)
      return;
    
    v = &__mgl_VertexBuffer[__mgl_VertexCount];
    __mgl_VertexCount++;
  }
  
  if (!v)
    return; // *UNREACHABLE
  
  v->x = x;
  v->y = y;
  v->z = z;
  v->r = __mgl_CurrentColor[0];
  v->g = __mgl_CurrentColor[1];
  v->b = __mgl_CurrentColor[2];
  v->a = __mgl_CurrentColor[3];
  v->u = __mgl_CurrentUV[0];
  v->v = __mgl_CurrentUV[1];
  v->nx = __mgl_CurrentNormals[0];
  v->ny = __mgl_CurrentNormals[1];
  v->nz = __mgl_CurrentNormals[2];
}

MGLAPI void mglVertex3fv(const GLfloat *v)
{
  mglVertex3f(v[0], v[1], v[2]);
}

MGLAPI void mglVertex2f(GLfloat x,GLfloat y)
{
  mglVertex3f(x, y, 0.0);
}

MGLAPI void mglNormal3f(GLfloat nx,GLfloat ny,GLfloat nz)
{
  __mgl_CurrentNormals[0] = nx;
  __mgl_CurrentNormals[1] = ny;
  __mgl_CurrentNormals[2] = nz;
}

MGLAPI void mglMatrixMode(GLenum mode)
{
  __mgl_MatrixMode = mode;
  switch (__mgl_MatrixMode) {
    case GL_PROJECTION:
      __mgl_CurrentMatrix = &__mgl_ProjectionMatrix;
      break;
    
    case GL_MODELVIEW:
      __mgl_CurrentMatrix = &__mgl_ModelViewMatrix;
      break;
  }
}

MGLAPI void mglLoadIdentity(void)
{
  *__mgl_CurrentMatrix = mat4x4_identity();
}

MGLAPI void mglFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
  double rl = right - left;
  double tb = top - bottom;
  double fn = zFar - zNear;

  MGLMatrix4x4 mat = {{
    { (2.0 * zNear) / rl, 0.0, (right + left) / rl, 0.0 },
    { 0.0, (2.0 * zNear) / tb, (top + bottom) / tb, 0.0 },
    { 0.0, 0.0, -(zFar + zNear) / fn, (-2.0 * zFar * zNear) / fn },
    { 0.0, 0.0, -1.0, 0.0 }
  }};

  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
}

MGLAPI void mglOrtho(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
  double rl = right - left;
  double tb = top - bottom;
  double fn = zFar - zNear;
  
  double tx = -(right+left)/rl;
  double ty = -(top+bottom)/tb;
  double tz = -(zFar+zNear)/fn;
  
  MGLMatrix4x4 mat = {{
    { 2.0/rl, 0.0,    0.0,     tx  },
    { 0.0,    2.0/tb, 0.0,     ty  },
    { 0.0,    0.0,    -2.0/fn, tz  },
    { 0.0,    0.0,    0.0,     1.0 }
  }};

  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
}