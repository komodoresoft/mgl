#ifndef __MGLU_H__
#define __MGLU_H__

#include <math.h>

#ifndef MGLAPI
#define MGLAPI
#endif // MGLAPI

MGLAPI void mgluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);
MGLAPI void mgluOrtho2D(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top);

#endif // __MGLU_H__