#include "../include/Renderer.h"

#include "../extern/glm/glm.hpp"
#include "../extern/glm/gtc/matrix_transform.hpp"
#include "../extern/glm/gtc/type_ptr.hpp"

#include "../include/GLCall.h"
#include "../include/Window.h"

const float Renderer::ASPECT_RATIO = (1.f * Renderer::INTERNAL_RESOLUTION_W) / Renderer::INTERNAL_RESOLUTION_H;

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
  PrintInfo();

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
  _primitivesShader = new Shader("../res/shaders/primitives_shader.vert",
                                 "../res/shaders/primitives_shader.frag");

  // Set orthographic projection to the standard normalized space
  // Setting a diferent projection requires a call to SetViewportSize()
  glm::mat4 ortho = glm::ortho(-1.f, 1.f, -1.f, 1.f);
  
  _spriteShader->Bind();
  _spriteShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

  _primitivesShader->Bind();
  _primitivesShader->SetUniformMat4("ortho", glm::value_ptr(ortho));

  return true;
}

void Renderer::Draw(Texture* tex, Rect* srcRect, Rect* dstRect) {
  if(!tex){
	  LOG_ERROR("Texture is NULL.");
	  DEBUG_BREAK();
	  return;
  }

  _spriteVAO->Bind();
  _spriteShader->Bind();
  tex->Bind();

  glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(dstRect->x() * _xScaleFactor,
                                                                 dstRect->y() * _yScaleFactor, 0.f));
  glm::mat4 scale = glm::scale(translate, glm::vec3(dstRect->w() * _xScaleFactor,
                                                    dstRect->h() * _yScaleFactor, 1.f));
  _spriteShader->SetUniformMat4("model", glm::value_ptr(scale));

  _spriteShader->SetUniform4f("srcrct", srcRect->x(), srcRect->y(), srcRect->w(), srcRect->h());

  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::DrawRect(Rect* rect, Color* color) {
  if(!rect){
	  LOG_ERROR("Rect is NULL.");
	  DEBUG_BREAK();
	  return;
  }
  if(!color){
	  LOG_ERROR("Color is NULL.");
	  DEBUG_BREAK();
	  return;
  }
		
  PreparePrimitiveForDrawing(rect, color);
  GLCall(glDrawArrays(GL_LINE_LOOP, 0, 4));
}

void Renderer::DrawFillRect(Rect* rect, Color* color) {
  if(!rect){
	  LOG_ERROR("Rect is NULL.");
	  DEBUG_BREAK();
	  return;
  }
  if(!color){
	  LOG_ERROR("Color is NULL.");
	  DEBUG_BREAK();
	  return;
  }

  PreparePrimitiveForDrawing(rect, color);
  GLCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

void Renderer::Clear() {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {

  GLCall(glClearColor(r, g, b, a));
}

void Renderer::SetViewportSize(int w, int h) {
  _viewportW = w;
  _viewportH = w / ASPECT_RATIO + 0.5f; // 0.5f is added to compensate for int truncation

  if(_viewportH > h){
    _viewportW = h * ASPECT_RATIO + 0.5f; // 0.5f is added to compensate for int truncation
    _viewportH = h;
  }

  // Centers the viewport on the screen
  int orthoX = (w - _viewportW) / 2;
  int orthoY = (h - _viewportH) / 2;

  glm::mat4 ortho = glm::ortho(0.f, (float)w, (float)h, 0.f);

  _spriteShader->Bind();
  _spriteShader->SetUniformMat4("ortho", value_ptr(ortho));

  _primitivesShader->Bind();
  _primitivesShader->SetUniformMat4("ortho", value_ptr(ortho));

  GLCall(glViewport(orthoX, orthoY, _viewportW, _viewportH));

  _xScaleFactor = ((float)w) / INTERNAL_RESOLUTION_W;
  _yScaleFactor = ((float)h) / INTERNAL_RESOLUTION_H;
}

void Renderer::PreparePrimitiveForDrawing(Rect* rect, Color* color) {
  _primitivesShader->Bind();
  _primitivesVAO->Bind();

  // Set transformation matrix
  glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(rect->x() * _xScaleFactor,
                                                                 rect->y() * _yScaleFactor, 0.f));
  glm::mat4 scale = glm::scale(translate, glm::vec3(rect->w() * _xScaleFactor,
                                                    rect->h() * _yScaleFactor,  0.f));

  _primitivesShader->SetUniformMat4("model", glm::value_ptr(scale));
  _primitivesShader->SetUniform4f("fragcolor", color->r, color->g, color->b, color->a);
}

void Renderer::PrintInfo() {
  // Prints current graphics device and OpenGL version
  const unsigned char* graphicsDeviceInfo = glGetString(GL_RENDERER);
  const unsigned char* openGLVersion = glGetString(GL_VERSION);
  const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

  std::cout << graphicsDeviceInfo << std::endl;
  std::cout << "OpenGL Version: "<< openGLVersion << std::endl;
  std::cout << "GLSL Version: " << glslVersion << std::endl;
}
