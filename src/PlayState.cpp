#include "../include/PlayState.h"

#include "../include/InfoMenu.h"
#include "../include/InputModule.h"
#include "../include/GameStateMachine.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Renderer.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_object->HandleInput();
	_infoMenu->HandleInput();

}

void PlayState::Update(){
    _object->Update();
	_camera->Update();
}

void PlayState::Render(Renderer* renderer, float deltatime){
	//TODO(Gustavo): Below is a temporary solution for the camera position interpolation problem.
	// This solution must be integrated properly withing the camera code
	Vector2D pos = _object->collisionRect().position();
	_object->setPosition(pos.x() + _object->velocity().x() * deltatime, 
				  		 pos.y() + _object->velocity().y() * deltatime); 
	_camera->Update();
	
	_object->setPosition(pos.x(), pos.y()); 
	
	_level->DrawBackground(renderer, deltatime);
	_object->Draw(renderer, deltatime);
	_infoMenu->Render(renderer);
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
		return false;
    }

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Standing.png", "maya_standing")){
	 	LOG_ERROR("Unable to load texture \"Maya_Standing\"");
		 return false;
	}

	PhysicsEngine::setCurrentLevel(_level);
	ServiceLocator::ProvideCurrentLevel(_level);
	ServiceLocator::ProvidePlayer(_object);

	ServiceLocator::GetRenderer()->UseCamera(_camera);

	_infoMenu = new InfoMenuGL3();

    return true;
}

bool PlayState::OnExit(){
	delete _level;
	delete _object;
	delete _camera;
	delete _infoMenu;

	ResourceManager::DeleteTexture("maya_running");
	ResourceManager::DeleteTexture("maya_standing");
	
    return true;
}
