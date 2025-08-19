#include <MGL/mgl.h>
#include <MGL/mglu.h>

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