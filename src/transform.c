#include <MGL/mgl.h>

extern MGLMatrix4x4 *__mgl_CurrentMatrix;

MGLAPI void mglScalef(GLfloat x,GLfloat y,GLfloat z)
{
  MGLMatrix4x4 mat;
  
  mat = mat4x4_identity();
  mat.m[0][0] = x;
  mat.m[1][1] = y;
  mat.m[2][2] = z;
  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
}

MGLAPI void mglTranslatef(GLfloat x,GLfloat y,GLfloat z)
{
  MGLMatrix4x4 mat;
  
  mat = mat4x4_identity();
  mat.m[0][3] = x;
  mat.m[1][3] = y;
  mat.m[2][3] = z;
  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
}

MGLAPI void mglRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
{
  MGLMatrix4x4 mat;
  GLfloat rad = angle * (M_PI / 180.0f);
  GLfloat c = cosf(rad);
  GLfloat s = sinf(rad);
  GLfloat one_minus_c = 1.0f - c;
  
  GLfloat len = sqrtf(x * x + y * y + z * z);
  if (len == 0.0f)
      return;
  x /= len;
  y /= len;
  z /= len;

  mat = mat4x4_identity();

  mat.m[0][0] = x * x * one_minus_c + c;
  mat.m[1][0] = x * y * one_minus_c - z * s;
  mat.m[2][0] = x * z * one_minus_c + y * s;

  mat.m[0][1] = y * x * one_minus_c + z * s;
  mat.m[1][1] = y * y * one_minus_c + c;
  mat.m[2][1] = y * z * one_minus_c - x * s;

  mat.m[0][2] = z * x * one_minus_c - y * s;
  mat.m[1][2] = z * y * one_minus_c + x * s;
  mat.m[2][2] = z * z * one_minus_c + c;
  
  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
}