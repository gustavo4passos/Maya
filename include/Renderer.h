#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Rect.h"
#include "Texture.h"


class Renderer {
public:
  static bool Init();
  static void Clear();
  static void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

  static void Draw(Texture rect, Rect srcRect, Rect dstRect);

private:
  Renderer() {}

};

#endif
