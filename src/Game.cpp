#include "../include/Game.h"

#include <SDL2/SDL.h>

#include "../include/AnimationPlayer.h"
#include "../include/GameStateMachine.h"
#include "../include/GameSwitches.h"
#include "../include/InputModule.h"
#include "../include/LevelFile.h"
#include "../include/Logger.h"
#include "../include/LuaScript.h"
#include "../include/PlayState.h"
#include "../include/ResourceManager.h"
#include "../include/SaveSystem.h"
#include "../include/SettingsManager.h"
#include "../include/ServiceLocator.h"

bool Game::Init() {
    _settingsManager = SettingsManager();
    int width = _settingsManager.WindowWidth();
    int height = _settingsManager.WindowHeight();
    bool vsync = _settingsManager.Vsync();
    bool fullscreen = _settingsManager.Fullscreen();

    _window = new Window("Maya", width, height, 3, 3, vsync, fullscreen);
    if(!_window->Init()) {
        LOG_ERROR("Unable to initialize window.");
        return false;
    }

    _renderer = new Renderer();
    if(!_renderer->Init()) {
        LOG_ERROR("Unable to initialize renderer.");
        return false;
    }

    _renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);
    _renderer->SetViewportSize(_window->width(), _window->height());

    if(!InputModule::Init()) {
        LOG_ERROR("Unable to initialize InputModule.");
        return false;
    }

    if(!SoundPlayer::Init()) {
        LOG_ERROR("Unable to initialize SoundPlayer.");
        return false;
    }
   
    ServiceLocator::ProvideGame(this);
    ServiceLocator::ProvideWindow(_window); 
    ServiceLocator::ProvideRenderer(_renderer);
    ServiceLocator::ProvideGameSwitches(new GameSwitches());
    ServiceLocator::ProvideSaveSystem(new SaveSystem());
    ServiceLocator::ProvideAnimationPlayer(new AnimationPlayer());
    ServiceLocator::ProvideSettingsManager(&_settingsManager);

    SoundPlayer::SetMasterVolume(_settingsManager.MasterVolume());
    
    GameStateMachine::PushState(new PlayState());
    
    _running = false;
    
    LevelFile level = LevelFile("temple-room-1.tmx");
    level.OpenFile();

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

void Game::Render(float deltaTime) {
	_renderer->Clear();
    GameStateMachine::Render(_renderer, deltaTime);
    _window->Swap();
}

void Game::Update() {
    GameStateMachine::Update();
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

void Game::EndGameRequest() {
	_window->SetFullscreen(false);
	_renderer->SetViewportSize(_window->width(), _window->height());
	if(_window->ShowQuitMessageBox()) _running = false;
}

void Game::Clean() {
    GameStateMachine::Clean();
	ResourceManager::Clean();
    InputModule::Clean();

    delete _renderer;
    delete _window;

    _renderer = nullptr;
    _window = nullptr; 
}

void Game::ChangeResolution(int width, int height) {
    _window->SetResolution(width, height);
    _renderer->SetViewportSize(width, height);
    _settingsManager.SetResolution(width, height);
}
	
void Game::SetVsync(bool active) {
    _window->SetVsync(active);
    _settingsManager.SetVsync(active);
}

void Game::SetFullscreen(bool active) {
    _window->SetFullscreen(active);
    _renderer->SetViewportSize(_window->width(), _window->height());
    _settingsManager.SetFullscreen(active);
} 
