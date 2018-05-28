#include "../include/Renderer.h"

#include "../extern/glm/glm.hpp"
#include "../extern/glm/gtc/matrix_transform.hpp"
#include "../extern/glm/gtc/type_ptr.hpp"

#include "../include/GLCall.h"

Renderer::Renderer() :
  _spriteVAO(NULL),
  _spriteVBO(NULL),
  _spriteShader(NULL) { }

Renderer::~Renderer() {
  delete _spriteVAO;
  delete _primitivesVAO;

  delete _spriteVBO;
  delete _primitivesVBO;

  delete _spriteShader;
  delete _primitivesShader;

  _spriteVAO = NULL;
  _spriteVBO = NULL;

  _primitivesVAO = NULL;
  _primitivesVBO = NULL;

  _spriteShader = NULL;
  _primitivesShader = NULL; 
}

bool Renderer::Init() {
  // Enable blend
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLCall(glEnable(GL_BLEND));

  // Generic vertex data on which model transformation can be made generically
  float spriteVertexData[] = {
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f, 
    1.f, 1.f, 1.f, 1.f,
    0.f, 1.f, 0.f, 1.f
  };

  float primitivesVertexData[] = {
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f
  };

  _spriteVBO = new VertexBuffer(spriteVertexData, sizeof(spriteVertexData));
  _primitivesVBO = new VertexBuffer(primitivesVertexData, sizeof(primitivesVertexData));

  VertexBufferLayout spriteVertexDataLayout = VertexBufferLayout();
  spriteVertexDataLayout.Push<float>(2);
  spriteVertexDataLayout.Push<float>(2);

  VertexBufferLayout primitivesVertexDataLayout = VertexBufferLayout();
  primitivesVertexDataLayout.Push<float>(2);

  _spriteVAO = new VertexArray();
  _spriteVAO->AddBuffer((*_spriteVBO), spriteVertexDataLayout);

  _primitivesVAO = new VertexArray();
  _primitivesVAO->AddBuffer((*_primitivesVBO), primitivesVertexDataLayout);

  _spriteShader = new Shader("../res/shaders/sprite_shader.vert", "../res/shaders/sprite_shader.frag");
  _primitivesShader = new Shader("../res/shaders/primitives_shader.vert", "../res/shaders/primitives_shader.frag");

  // Set orthographic projection to the standard normalized space
  // Setting a diferent projection requires a call to SetViewportSize()
  glm::mat4 ortho = glm::ortho(-1.f, 1.f, -1.f, 1.f);
  _spriteShader->SetUniformMat4("ortho", glm::value_ptr(ortho));
  _primitivesShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

  return true;
}

void Renderer::Draw(Texture* tex, Rect* srcRect, Rect* dstRect) {

  _spriteVAO->Bind();
  _spriteShader->Bind();
  tex->Bind();

  glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(dstRect->x() * 1.f, dstRect->y() * 1.f, 0.f));
  glm::mat4 scale = glm::scale(translate, glm::vec3(dstRect->w() * 1.f, dstRect->h() * 1.f, 1.f));
  _spriteShader->SetUniformMat4("model", glm::value_ptr(scale));

  _spriteShader->SetUniform4f("srcrct", srcRect->x(), srcRect->y(), srcRect->w(), srcRect->h());
  
  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::DrawRect(Rect* rect, Color* color) {
  PreparePrimitiveForDrawing(rect, color);
  GLCall(glDrawArrays(GL_LINE_LOOP, 0, 4));
}

void Renderer::DrawFillRect(Rect* rect, Color* color) {

  PreparePrimitiveForDrawing(rect, color);
  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::Clear() {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {

  GLCall(glClearColor(r, g, b, a));
}

void Renderer::SetViewportSize(float w, float h) {
  _viewportW = w;
  _viewportH = h;
  
  glm::mat4 ortho = glm::ortho(0.f, w, h, 0.f);

  _spriteShader->SetUniformMat4("ortho", value_ptr(ortho));
  _primitivesShader->SetUniformMat4("ortho", value_ptr(ortho));
}

void Renderer::PreparePrimitiveForDrawing(Rect* rect, Color* color) {
  _primitivesShader->Bind();
  _primitivesVAO->Bind();

  // Set transformation matrix
  glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(rect->x(), rect->y(), 0.f));
  glm::mat4 scale = glm::scale(translate, glm::vec3(rect->w() * 1.f, rect->h() * 1.f, 0.f));

  _primitivesShader->SetUniformMat4("model", glm::value_ptr(scale));
  _primitivesShader->SetUniform4f("fragcolor", color->r, color->g, color->b, color->a);
}
