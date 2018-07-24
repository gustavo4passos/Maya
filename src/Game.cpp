#include "../include/Game.h"

#include <SDL2/SDL.h>

#include "../include/LuaScript.h"
#include "../include/ErrorHandler.h"
#include "../include/InputModule.h"
#include "../include/Maya.h"
#include "../include/ResourceManager.h"

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

    _renderer->SetClearColor(0.f, .8f, 0.f, 1.f);
    _renderer->SetViewportSize(_window->width(), _window->height());

    if(!InputModule::Init()){
        LOG_ERROR("Unable to initialize InputModule.");
        return false;
    }
   
    if(!ResourceManager::LoadTexture("../res/assets/Maya_Run Sprite Sheet V03.png", "maya_run")) {
        LOG_ERROR("Unbale to load texture.");
    }
    if(!ResourceManager::LoadTexture("../res/assets/Maya_Combat_Attack_v02.png", "maya_attack")) {
        LOG_ERROR("Unbale to load texture.");
    }
    if(!ResourceManager::LoadTexture("../res/assets/Maya_Jump_V01.png", "maya_jump")) {
        LOG_ERROR("Unbale to load texture.");
    }
    if(!ResourceManager::LoadTexture("../res/assets/Maya_Stand Arms.png", "maya_stand")) {
        LOG_ERROR("Unbale to load texture.");
    }

    _maya = new Maya();
    _maya->Load(270,100,36,39,"maya_stand");

    _running = false;

    return true;
}

void Game::Run() {
    _running = true;

    unsigned int previous = SDL_GetTicks();
    unsigned int lag = 0.0;
    const unsigned int MS_PER_UPDATE = 16;
  
    while(_running) {
        unsigned int current =  SDL_GetTicks();
        unsigned int elapsed = current - previous;

        previous = current;
        lag += elapsed;
        
                

        while(lag >= MS_PER_UPDATE){
            HandleEvents();
            Update();
            lag -= MS_PER_UPDATE;            
        }

        Render(float(lag) / MS_PER_UPDATE);
    }
    
}

void Game::Render(float positionFactor) {
    _renderer->Clear();
    _maya->Draw(_renderer, positionFactor);
    _window->Swap();
}

void Game::Update() {
    _maya->Update();
}

void Game::Clean() {
    InputModule::Clean();
	  ResourceManager::CleanTextures();

    delete _renderer;  
    delete _window;

    _renderer = NULL;
    _window = NULL;
}

void Game::HandleEvents() {
    
    InputModule::Update();

    if(InputModule::CloseWindowRequest() ||
       InputModule::WasKeyReleased(InputModule::ESC)){
        _window->SetFullscreen(false);
        _renderer->SetViewportSize(_window->width(), _window->height());
        if(_window->ShowQuitMessageBox()) _running = false;
    }
    if(InputModule::IsKeyPressed(InputModule::LALT) && 
       InputModule::WasKeyReleased(InputModule::ENTER)) {
           _window->ToggleFullscreen();
           _renderer->SetViewportSize(_window->width(), _window->height());
    }

    _maya->HandleInput();
}
