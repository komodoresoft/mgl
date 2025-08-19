#include <MGL/mgl.h>
#include <MGL/displaylist.h>

extern MGLDisplayList *__mgl_DisplayLists[MAX_DISPLAYLISTS];
extern MGLDisplayList *__mgl_CurrentDisplayList;
extern GLint __mgl_ModelViewUni;
extern GLint __mgl_ProjectionUni;
extern GLuint __mgl_program;
extern GLuint __mgl_VBO, __mgl_VAO;
extern MGLMatrix4x4 __mgl_ModelViewMatrix;
extern MGLMatrix4x4 __mgl_ProjectionMatrix;

MGLAPI GLuint mglGenLists(GLsizei range)
{
  GLsizei i;
  GLuint base;
  int found = 0;
  
  if (range <= 0)
    return 0;
  
  for (i = 1; i < MAX_DISPLAYLISTS; ++i) {
    if (!__mgl_DisplayLists[i]) { // * = NULL
      found = 1;
      for (GLsizei j = 0; j < range; ++j) {
        if (__mgl_DisplayLists[i + j]) { // range not clear
          found = 0;
          break;
        }
      }
      if (found)
        break;
    }
  }
  
  if (!found)
    return 0;
  
  base = i;
  for (int j = 0; j < range; ++j) {
    MGLDisplayList **dl = &__mgl_DisplayLists[base + j];
    *dl = malloc(sizeof(MGLDisplayList));
    if (!*dl) {
      for (int j = 0; j < range; ++j) {
        // free all if one failed
        if (__mgl_DisplayLists[base + j]) free(__mgl_DisplayLists[base + j]);
        __mgl_DisplayLists[base + j] = NULL;
      }
      return 0;
    }
    (*dl)->drawmode = GL_TRIANGLES;
    (*dl)->count = 0;
  }
  return base;
}

MGLAPI void mglNewList(GLuint list,GLenum mode)
{
  (void) mode;
  
  if (list <= 0 || list >= MAX_DISPLAYLISTS)
    return;
    
  __mgl_CurrentDisplayList = __mgl_DisplayLists[list];
}

MGLAPI void mglEndList(void)
{
  __mgl_CurrentDisplayList = NULL;
}

MGLAPI void mglCallList(GLuint list)
{
  if (list <= 0 || list >= MAX_DISPLAYLISTS)
    return;
  
  MGLDisplayList *dl = __mgl_DisplayLists[list];
  if (!dl)
    return;
  
  glUseProgram(__mgl_program);
  glBindVertexArray(__mgl_VAO);
  
  glUniformMatrix4fv(__mgl_ModelViewUni, 1, GL_TRUE, &__mgl_ModelViewMatrix.m[0][0]);
  glUniformMatrix4fv(__mgl_ProjectionUni, 1, GL_TRUE, &__mgl_ProjectionMatrix.m[0][0]);
  
  glBindBuffer(GL_ARRAY_BUFFER, __mgl_VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, dl->count * sizeof(MGLVertex), dl->vtxbuffer);
  glDrawArrays(dl->drawmode, 0, dl->count);
}

MGLAPI void mglDeleteLists(GLuint list,GLsizei range)
{
  if (list < 0 || list >= MAX_DISPLAYLISTS)
    return;
  
  if (range < 0)
    return;
  
  for (GLuint j = 1; j < range; ++j) {
    MGLDisplayList **dl = &__mgl_DisplayLists[list + j];
    if (j >= MAX_DISPLAYLISTS)
      break;
    if (*dl) free(*dl);
    *dl = NULL;
  }
}