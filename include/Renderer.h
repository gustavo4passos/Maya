#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Rect.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

struct Color {
  float r;
  float g;
  float b;
};


// WARNING: 
// The renderer requires a valid OpenGL context to do anything relevant.
// Trying to initialize or render without a valid OpenGL context is going to 
// cause an infinite loop due to how the error system works (GLCall()) when in
// debug mode (Activated by defining M_DEBUG as a preprocessor directive)


class Renderer {
public:
  Renderer();
  ~Renderer();

  // Initializes the renderer data
  bool Init();
  void Clean();
  void Clear();
  void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

  void Draw(Texture& tex, Rect& srcRect, Rect& dstRect);
  void Draw(Texture tex, Rect srcRect, Rect dstRect, float scale, float angle = 0);

  void DrawRect(Rect& rect, Color& color);
  void DrawFillRect(Rect& rect, Color& color);

  void SetViewportSize(float w, float h);

private:
  VertexArray* _spriteVAO;
  VertexBuffer* _spriteVBO;
  Shader* _spriteShader;

  float _viewportW, _viewportH;  
};

#endif
