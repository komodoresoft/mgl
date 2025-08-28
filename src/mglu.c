#include <MGL/mgl.h>
#include <MGL/mglu.h>

extern MGLMatrix4x4 *__mgl_CurrentMatrix;

MGLAPI void mgluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar)
{
  GLdouble rad = fovy * M_PI / 180.0;
  GLdouble top = zNear * tan(rad / 2.0);
  GLdouble bottom = -top;
  GLdouble right = top * aspect;
  GLdouble left = -right;
  mglFrustum(left, right, bottom, top, zNear, zFar);
}

MGLAPI void mgluOrtho2D(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top)
{
  mglOrtho(left, right, bottom, top, -1.0, 1.0);
}

MGLAPI void mgluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz)
{
  MGLMatrix4x4 mat;
  float3 forward, up, side;
  
  forward.x = centerx - eyex;
  forward.y = centery - eyey;
  forward.z = centerz - eyez;
  float3_normalize(&forward);
  
  up.x = upx;
  up.y = upy;
  up.z = upz;
  float3_normalize(&up);  
  
  float3_cross(&side, forward, up);
  float3_normalize(&side);
  
  float3_cross(&up, side, forward);
  
  mat = mat4x4_identity();
  mat.m[0][0] = side.x;
  mat.m[0][1] = side.y;
  mat.m[0][2] = side.z;

  mat.m[1][0] = up.x;
  mat.m[1][1] = up.y;
  mat.m[1][2] = up.x;

  mat.m[2][0] = -forward.x;
  mat.m[2][1] = -forward.y;
  mat.m[2][2] = -forward.z;

  *__mgl_CurrentMatrix = mat4x4_mul(*__mgl_CurrentMatrix, mat);
  mglTranslatef(-eyex, -eyey, -eyez);
}