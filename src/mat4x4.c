#include <MGL/mat4x4.h>

MGLAPI void float3_normalize(float3 *v)
{
  float len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  if (len == 0.0) {
    v->x = 0.0;
    v->y = 0.0;
    v->z = 0.0;
  } else {
    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}

MGLAPI void float3_cross(float3 *d, float3 a, float3 b)
{
  d->x = a.y * b.z - a.z * b.y;
  d->y = a.z * b.x - a.x * b.z;
  d->z = a.x * b.y - a.y * b.x;
}

MGLAPI MGLMatrix4x4 mat4x4_identity(void)
{
  MGLMatrix4x4 ret;
  
  memset(&ret, '\0', sizeof(ret));
  ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = ret.m[3][3] = 1.0;
  return ret;
}

MGLAPI MGLMatrix4x4 mat4x4_mul(MGLMatrix4x4 a, MGLMatrix4x4 b)
{
  MGLMatrix4x4 ret;

  memset(&ret, '\0', sizeof(ret));
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      for (int k = 0; k < 4; ++k)
        ret.m[i][j] += a.m[i][k] * b.m[k][j];

  return ret;
}

MGLAPI float4 mat4x4_float4_mul(float4 a, MGLMatrix4x4 b)
{
  float4 ret;
  
  for (int i = 0; i < 4; ++i) {
    ret.v[i] = 0.0;
    for (int j = 0; j < 4; ++j) {
      ret.v[i] += b.m[i][j] * a.v[j];
    }
  }
  return ret;
}