#include "../include/Game.h"

#include <SDL2/SDL.h>

#include "../include/Tileset.h"
#include "../include/LuaScript.h"
#include "../include/ErrorHandler.h"
#include "../include/InputModule.h"
#include "../include/Maya.h"
#include "../include/ResourceManager.h"
#include "../include/Mesh.h"
#include "../include/PhysicsEngine.h"
#include "../include/InfoMenu.h"

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

	_object = new GameObject(30, 0, 32, 32);
	_level = ResourceManager::ParseLevel("../res/levels/mario.tmx");
	_camera = new Camera(480, 270, 0, _level->width() * _level->tileWidth(), 0, _level->height() * _level->tileHeight(), _object);

    _renderer = new Renderer();
    if(!_renderer->Init(_camera)){
        LOG_ERROR("Unable to initialize renderer.");
        return false;
    }

    _renderer->SetClearColor(0.f, .8f, 0.f, 1.f);
    _renderer->SetViewportSize(_window->width(), _window->height());

    if(!InputModule::Init()){
        LOG_ERROR("Unable to initialize InputModule.");
        return false;
    }

    if(!ResourceManager::LoadTexture("../res/assets/Maya_Stand_Run2_Sprite_Sheet_x1_V02-1row.png", "maya_running")) {
        LOG_ERROR("Unbale to load texture.");
    }

    if(!ResourceManager::LoadTexture("../res/sprites/grass.png", "grass")){
		LOG_ERROR("Unable to load texture.");
	}

    _maya = new Maya();
    _maya->Load(270, 100, 36, 39, "maya_running");
	

    _running = false;
  
	PhysicsEngine::setCurrentLevel(_level);

	_infoMenu = new InfoMenuGL3(this, _window, _level, _maya, _object);

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
	_renderer->UseOffscreenFramebuffer();
	_renderer->Clear();
	
	_level->DrawBackground(_renderer, positionFactor);
	_maya->Draw(_renderer, positionFactor);
	_object->Draw(_renderer, positionFactor);

	_renderer->UseDefaultFramebuffer();
	_renderer->Clear();
	
	_renderer->RenderOffscreenFramebuffer();
	_infoMenu->Render(_renderer);

    _window->Swap();
}

void Game::Update() {
	_object->Update();
    _maya->Update();
	_camera->Update();
}

void Game::Clean() {
	delete _object;
	delete _maya;
	ResourceManager::CleanTextures();
	ResourceManager::CleanMeshes();
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

	_infoMenu->HandleInput();
    _maya->HandleInput();
	_object->HandleInput();
}
