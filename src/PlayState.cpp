#include "../include/PlayState.h"
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

    //---------incomplete---------
}

void PlayState::Render(Renderer* renderer, float deltatime){
	//Vector2D pos = Vector2D(_object->x(), _object->y());

	//TODO(Gustavo): Below is a temporary solution for the camera position interpolation problem.
	// This solution must be integrated properly withing the camera code
	
	//_object->setPosition(pos.x() + _object->velocity().x() * deltatime, 
	//			  		 pos.y() + _object->velocity().y() * deltatime); 
	//_camera->Update();
	//_object->setPosition(pos.x(), pos.y()); 

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
    }

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Standing.png", "maya_standing")){
	 	LOG_ERROR("Unable to load texture \"Maya_Standing\"");
	}

	PhysicsEngine::setCurrentLevel(_level);
	ServiceLocator::ProvideCurrentLevel(_level);
	ServiceLocator::ProvidePlayer(_object);

	ServiceLocator::GetRenderer()->UseCamera(_camera);

	_infoMenu = new InfoMenuGL3();

    return true;
    //----------waiting for gustavo to teach us how to use LuaScript---------
}

bool PlayState::OnExit(){
    return true;
    //----------clear the textures------------
}
