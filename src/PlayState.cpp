#include "../include/PlayState.h"
#include "../include/InputModule.h"
#include "../include/GameStateMachine.h"
#include "../include/Game.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_object->HandleInput();
}

void PlayState::Update(){
    _object->Update();
	_camera->Update();

    //---------incomplete---------
}

void PlayState::Render(Renderer* renderer){
	
	_level->DrawBackground(renderer, 1.f);
	_object->Draw(renderer, 1.f);

    //-----------waiting for vinicius------------
}

bool PlayState::OnEnter(){
    _level = ResourceManager::ParseLevel("../res/levels/forest.tmx");
	if(_level == NULL){
	 	return false;
	}

	_object = new GameObject(30, 0, 36, 39);
	_camera = new Camera(480, 270, 0, _level->width() * _level->tileWidth(), 0, _level->height() * _level->tileHeight(), _object);


    if(!ResourceManager::LoadTexture("../res/assets/Maya_Stand_Run2_Sprite_Sheet_x1_V02-1row.png", "maya_running")) {
        LOG_ERROR("Unbale to load texture.");
    }

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Standing.png", "maya_standing")){
	 	LOG_ERROR("Unable to load texture \"Maya_Standing\"");
	}

	PhysicsEngine::setCurrentLevel(_level);

    return true;
    //----------waiting for gustavo to teach us how to use LuaScript---------
}

bool PlayState::OnExit(){
    return true;
    //----------clear the textures------------
}
