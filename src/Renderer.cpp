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
  delete _spriteVBO;
  delete _spriteShader;
}  

bool Renderer::Init() {
  // Enable blend
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLCall(glEnable(GL_BLEND));

  float data[] = {
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f, 
    1.f, 1.f, 1.f, 1.f,
    0.f, 1.f, 0.f, 1.f
  };

  _spriteVBO = new VertexBuffer(data, sizeof(data));
  VertexBufferLayout layout = VertexBufferLayout();
  layout.Push<float>(2);
  layout.Push<float>(2);
  _spriteVAO = new VertexArray();
  _spriteVAO->AddBuffer((*_spriteVBO), layout);
  _spriteShader = new Shader("../res/shaders/sprite_shader.vert", "../res/shaders/sprite_shader.frag");

  // Set orthographic projection to the standard normalized space
  glm::mat4 ortho = glm::ortho(-1.f, 1.f, -1.f, 1.f);
  _spriteShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

  return true;
}

void Renderer::Draw(Texture* tex, Rect* srcRect, Rect* dstRect) {

  _spriteVAO->Bind();
  _spriteShader->Bind();
  tex->Bind();

  glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(dstRect->x() * 1.f, dstRect->y() * 1.f, 1.f));
  glm::mat4 scale = glm::scale(translate, glm::vec3(dstRect->w() * 1.f, dstRect->h() * 1.f, 1.f));
  _spriteShader->SetUniformMat4("model", glm::value_ptr(scale));

  _spriteShader->SetUniform4f("srcrct", srcRect->x(), srcRect->y(), srcRect->w(), srcRect->h());
  
  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::DrawRect(Rect* rect, Color* color) {

  _spriteVAO->Bind();
  _spriteShader->Bind();

  // Set transformation matrix
  glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(rect->w() * 1.f, rect->h() * 1.f, 1.f));
  _spriteShader->SetUniformMat4("model", glm::value_ptr(scale));
  _spriteShader->SetUniform3f("color", color->r, color->g, color->b);

  GLCall(glDrawArrays(GL_LINE_LOOP, 0, 4));
}

void Renderer::DrawFillRect(Rect* rect, Color* color) {

  _spriteVAO->Bind();
  _spriteShader->Bind();

  // Set transformation matrix
  glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(rect->w() * 1.f, rect->h() * 1.f, 1.f));
  _spriteShader->SetUniformMat4("model", glm::value_ptr(scale));
  _spriteShader->SetUniform3f("color", color->r, color->g, color->b);

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
}