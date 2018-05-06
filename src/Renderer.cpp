#include "../include/Renderer.h"

void Renderer::Clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  glClearColor(r, g, b, a);
}
