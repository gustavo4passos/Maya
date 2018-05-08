#include <iostream>

#include "../include/IndexBuffer.h"
#include "../include/InputModule.h"
#include "../include/LuaScript.h"
#include "../include/Renderer.h"
#include "../include/Shader.h"
#include "../include/VertexBuffer.h"
#include "../include/Window.h"

int main(int argc, char *argv[]) {

  LuaScript script = LuaScript("../res/config.lua");
  int width = script.Get<int>("window.w");
  int height = script.Get<int>("window.h");
  bool vsync = script.Get<bool>("window.vsync");
  bool fullscreen = script.Get<bool>("window.fullscreen");

  Window window = Window("Project Maya", width, height, 3, 2, vsync, fullscreen);
  window.Init();
  InputModule::Init();

  Shader shader = Shader("../res/shaders/default_vs.vs", "../res/shaders/default_fs.fs");
  shader.Bind();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(1);

  GLfloat vertices[] = {
    -1.f,  1.f,
     1.f,  1.f,
     1.f, -1.f,
    -1.f, -1.f
  };

  VertexBuffer vbo = VertexBuffer((GLvoid*)vertices, sizeof(GLfloat) * 8);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

  Renderer::SetClearColor(.3f, .3f, .3f, 1.f);

  while(true){

    InputModule::Update();
    if(InputModule::WasKeyReleased(InputModule::ESC)) {
      break;
    }
    if(InputModule::CloseWindowRequest()) {
      break;
    }

    shader.SetUniform1f("time", (float)SDL_GetTicks() / 1000);
    shader.SetUniform2f("resolution", width, height);

    Renderer::Clear();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    while(GLenum error = glGetError()) std::cout << gluErrorString(error) << std::endl;
    window.Swap();
  }

  window.Close();

  return 0;
}
