#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

class Renderer {
public:
  static void Clear();
  static void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

private:
  Renderer() {}

};

#endif
