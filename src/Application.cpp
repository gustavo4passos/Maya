#include <iostream>

#include "../include/IndexBuffer.h"
#include "../include/InputModule.h"
#include "../include/LuaScript.h"
#include "../include/Renderer.h"
#include "../include/Shader.h"
#include "../include/VertexArray.h"
#include "../include/Window.h"

int main(int argc, char *argv[]) {

  LuaScript script = LuaScript("../res/config.lua");
  int width = script.Get<int>("window.w");
  int height = script.Get<int>("window.h");
  bool vsync = script.Get<bool>("window.vsync");
  bool fullscreen = script.Get<bool>("window.fullscreen");

  Window window = Window("Project Maya", width, height, 3, 3, vsync, fullscreen);
  window.Init();
  InputModule::Init();

  float vertices[] = {
    -0.5f,  0.5f, 1.f, 0.f, 0.f,
     0.5f, -0.5f, 0.f, 1.f, 0.f,
    -0.5f, -0.5f, 0.f, 0.f, 1.f,
     0.5f,  0.5f, .3f, .6f, .9f
  };

  unsigned char indices[] = { 0, 2, 1, 3 };

  VertexArray vao = VertexArray();
  VertexBuffer vbo = VertexBuffer(vertices, sizeof(vertices));
  VertexBufferLayout layout = VertexBufferLayout();
  layout.Push<float>(2);
  layout.Push<float>(3);
  vao.AddBuffer(vbo, layout);
  
  Shader shader = Shader("../res/shaders/default_vs.vs", "../res/shaders/default_fs.fs");
  shader.Bind();

  Renderer::SetClearColor(.3f, .3f, .3f, 1.f);

  while(true) {
    
    InputModule::Update();
    if(InputModule::WasKeyReleased(InputModule::ESC)) {
      break;
    }
    if(InputModule::CloseWindowRequest()) {
      break;
    }

    Renderer::Clear();
    while(GLenum error = glGetError()) std::cout << gluErrorString(error) << std::endl;    
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, indices);   
    while(GLenum error = glGetError()) std::cout << gluErrorString(error) << std::endl;
    window.Swap();
  }

  window.Close();

  return 0;
}
