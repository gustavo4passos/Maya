#include "../include/Game.h"
#include "../include/LuaScript.h"
#include "../include/ErrorHandler.h"
#include "../include/InputModule.h"
#include "../include/Maya.h"

#include <SDL2/SDL.h>

bool Game::Init() {
    LuaScript lua = LuaScript("../res/config.lua");
    int width = lua.Get<int>("window.w");
    int height = lua.Get<int>("window.h");
    bool vsync = lua.Get<bool>("window.vsync");
    bool fullscreen = lua.Get<bool>("window.fullscreen");

    _window = new Window("Maya", width, height, 3, 3, vsync, fullscreen);
    if(!_window->Init()){
        LOG_ERROR("Unable to initialize window.");
        return false;
    }
    
    _renderer = new Renderer();
    if(!_renderer->Init()){
        LOG_ERROR("Unable to initialize renderer.");
        return false;
    }

    _renderer->SetClearColor(0.f, 1.f, 0.f, 1.f);
    _renderer->SetViewportSize(width, height);

    if(!InputModule::Init()){
        LOG_ERROR("Unable to initialize InputModule.");
        return false;
    }

    _maya = new Maya();
    _maya->Load(0,height/2,72,76,"../Maya_More_Clothes.png",3);

    _running = false;

    return true;
}

void Game::Run() {
    _running = true;

    while(_running) {
        Update();
        Render();

    }
}

void Game::Render() {
    _renderer->Clear();

    _maya->Draw(_renderer);

    _window->Swap();
}

void Game::Update() {
    unsigned int timePassed = SDL_GetTicks();
	unsigned int frameTime = timePassed - _lastFrame;
	_lastFrame = timePassed;
    
    InputModule::Update();
    
    _maya->Update(frameTime);
    
    HandleEvents();
}

void Game::Clean() {
     delete _renderer;

    _window->Close();
    InputModule::Clean();
    
   
    delete _window;

    _renderer = NULL;
    _window = NULL;
}

void Game::HandleEvents() {
    if(InputModule::CloseWindowRequest()){
        _running = false;
    }
    if(InputModule::WasKeyReleased(InputModule::ESC)){
        _running = false;
    }
}

