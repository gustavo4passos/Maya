#include <iostream>

#include "../include/Window.h"
#include "../include/LuaScript.h"
#include "../include/Shader.h"
#include "../include/Renderer.h"

int main(int argc, char *argv[]) {

  LuaScript script = LuaScript("../res/config.lua");

  int width = script.Get<int>("window.w");
  int height = script.Get<int>("window.h");
  bool vsync = script.Get<bool>("window.vsync");
  bool fullscreen = script.Get<bool>("window.fullscreen");

  Window window = Window("Game", width, height, 3, 2, vsync, fullscreen);
  window.Init();

  Renderer::SetClearColor(.3f, .3f, .3f, 1.f);

  bool running = true;
  while(running){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT) { running = false; }
      if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) { running = false; }
      }

    Renderer::Clear();
    window.Swap();
  }

  window.Close();
  return 0;
}
