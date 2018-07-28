#include "../include/Game.h"

#include <thread>

#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "../include/ErrorHandler.h"
#include "../include/GameStateMachine.h"
#include "../include/GameSwitches.h"
#include "../include/InputModule.h"
#include "../include/LuaScript.h"
#include "../include/ResourceManager.h"
#include "../include/SoundPlayer.h"
#include "../include/PhysicsEngine.h"
#include "../include/ServiceLocator.h"
#include "../include/PlayState.h"

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

    _renderer->SetClearColor(0.09f, .60f, 0.85f, 1.f);
    _renderer->SetViewportSize(_window->width(), _window->height());

    if(!InputModule::Init()){
        LOG_ERROR("Unable to initialize InputModule.");
       return false;
    }

    //if(!InputModule::InitJoysticks()){
    //   LOG_ERROR("Unable to initialize Joysticks");
    //}

    if(!SoundPlayer::Init()){
        LOG_ERROR("Unable to initialize SoundPlayer.");
        return false;
    }
   
    ServiceLocator::ProvideGame(this);
    ServiceLocator::ProvideWindow(_window); 
    ServiceLocator::ProvideRenderer(_renderer);

    ServiceLocator::ProvideGameSwitches(new GameSwitches());
    ServiceLocator::GetGameSwitches()->PushSwitch("forest-button-1");
    ServiceLocator::GetGameSwitches()->PushSwitch("mountain-switch-10");
    GameStateMachine::PushState(new PlayState());
    
    _running = false;
    return true;
}

void Game::Run() {
    _running = true;

    unsigned int previous = SDL_GetTicks();
    unsigned int lag = 0.0;
    const unsigned int MS_PER_UPDATE = 16;

    while(_running) {
        unsigned int current = SDL_GetTicks();
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

    GameStateMachine::Render(_renderer, positionFactor);

    _window->Swap();
}

void Game::Update() {
    GameStateMachine::Update();
}

void Game::Clean() {
    GameStateMachine::Clean();
	ResourceManager::Clean();
    InputModule::Clean();

    delete _renderer;
    delete _window;

    _renderer = NULL;
    _window = NULL;
}

void Game::EndGameRequest() {
	_window->SetFullscreen(false);
	_renderer->SetViewportSize(_window->width(), _window->height());
	if(_window->ShowQuitMessageBox()) _running = false;
}
	
void Game::HandleEvents() {
    InputModule::Update();

    if(InputModule::CloseWindowRequest()) { 
		EndGameRequest();
    }

    if(InputModule::IsKeyPressed(InputModule::LALT) && 
       InputModule::WasKeyReleased(InputModule::ENTER)) {
           _window->ToggleFullscreen();
           _renderer->SetViewportSize(_window->width(), _window->height());
    }

    GameStateMachine::HandleInput();
}
