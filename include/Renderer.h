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
  float a;
};

class Window;

// Trying to initialize or render without a valid OpenGL context is going to 
// cause an infinite loop due to how the error log system works (GLCall()) when in
// debug mode (Activated by defining M_DEBUG as a preprocessor directive)

class Renderer {
public:
  Renderer();
  ~Renderer();
  void Clean();

  // Initializes the renderer data
  bool Init();
  void Clear();
  void SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

  void Draw(Texture* tex, Rect* srcRect, Rect* dstRect);
  void Draw(Texture tex, Rect srcRect, Rect dstRect, float scale, float angle = 0);

  void DrawRect(Rect* rect, Color* color);
  void DrawFillRect(Rect* rect, Color* color);

  void SetViewportSize(int w, int h);

private:
  Window* _windowPtr;

  VertexArray* _spriteVAO;
  VertexArray* _primitivesVAO;
  VertexBuffer* _spriteVBO;
  VertexBuffer* _primitivesVBO;
  Shader* _spriteShader;
  Shader* _primitivesShader;

  int _viewportW, _viewportH;  

  void PreparePrimitiveForDrawing(Rect* rect, Color* color);

  //Print info about the current graphics device and glsl version
  void PrintInfo();

  // Internal resolution data
  static const int INTERNAL_RESOLUTION_W = 480;
  static const int INTERNAL_RESOLUTION_H = 270;
  static const float ASPECT_RATIO;
  // static const float ASPECT_RATIO = (1.f * INTERNAL_RESOLUTION_W) / INTERNAL_RESOLUTION_H;

  float _xScaleFactor, _yScaleFactor;
};

#endif
