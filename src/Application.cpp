#include <iostream>
#include "../include/IndexBuffer.h"
#include "../include/InputModule.h"
#include "../include/LuaScript.h"
#include "../include/Renderer.h"
#include "../include/Shader.h"
#include "../include/VertexArray.h"
#include "../include/Window.h"
#include "../include/Texture.h"

int main(int argc, char *argv[]) {

  LuaScript script = LuaScript("../res/config.lua");
  int width = script.Get<int>("window.w");
  int height = script.Get<int>("window.h");
  bool vsync = script.Get<bool>("window.vsync");
  bool fullscreen = script.Get<bool>("window.fullscreen");

  Window window = Window("Project Maya", width, height, 3, 3, vsync, fullscreen);

  window.Init(); 
  {
    InputModule::Init();

    Renderer renderer = Renderer();
    renderer.Init();
    renderer.SetClearColor(.3f, .3f, .3f, 1.f);
    renderer.SetViewportSize(width, height);

    Texture tex = Texture("../res/sprites/emma.jpg");

    while(true) {

      InputModule::Update();

      if(InputModule::WasKeyReleased(InputModule::ESC)) {
        break;
      }
      if(InputModule::CloseWindowRequest()) {
        break;
      }

      renderer.Clear();
      Rect r = Rect(0, 0, 800, 600);
      renderer.Draw(tex, r, r);
      window.Swap();
    }
  }

  window.Close();

  return 0;
}
