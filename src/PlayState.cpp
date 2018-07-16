#include "../include/PlayState.h"

#include "../include/Button.h"
#include "../include/Door.h"
#include "../include/EventDispatcher.h"
#include "../include/EvilSonic.h"
#include "../include/GameSwitches.h"
#include "../include/GameStateMachine.h"
#include "../include/InfoMenu.h"
#include "../include/InputModule.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Region.h"
#include "../include/Renderer.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_object->HandleInput();
	_infoMenu->HandleInput();

}

void PlayState::Update(){
	_region->Update();
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
	
	_region->Render(renderer, deltatime);
	_object->Draw(renderer, deltatime);
	_infoMenu->Render(renderer);
}

bool PlayState::OnEnter(){
	 if(!ResourceManager::LoadTexture("../res/assets/Maya_Stand_Run2_Sprite_Sheet_x1_V02-1row.png", "maya_running")) {
        LOG_ERROR("Unbale to load texture.");
		return false;
    }

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Standing.png", "maya_standing")){
	 	LOG_ERROR("Unable to load texture \"Maya_Standing\"");
		 return false;
	}

	if(!ResourceManager::LoadTexture("../res/sprites/button.png", "button")) {
		LOG_ERROR("Unable to load texture \"button\"");
		return false;
	}

	if(!ResourceManager::LoadTexture("../res/sprites/door.png", "door")) {
		LOG_ERROR("Unable to load texture \"Door\"");
		return false;
	}
	
	_region = new Region();
	ServiceLocator::ProvideCurrentRegion(_region);
	
	_object = new GameObject(200, 0, 36, 39, 12, 7);
	ServiceLocator::ProvidePlayer(_object);

	_infoMenu = new InfoMenuGL3();

	Level* forest = ResourceManager::ParseLevel("../res/levels/forest.tmx");
	forest->AddEnemy(new EvilSonic(10, 100, 36, 39, 12, 5));
	forest->AddGameObject(new Button(130, 514, 32, 32, 1, 10, 31, 22, "forest-button-1", false));
	forest->AddGameObject(new Door(384, 450, 32, 32, 0, 0, 32, 32, "forest-button-1"));

	if(forest == NULL) return false;

	Level* mountain = ResourceManager::ParseLevel("../res/levels/mountain.tmx");

	_region->AddLevel(forest, "forest");
	_region->AddLevel(mountain, "mountain");
	_region->ChangeCurrentLevel("forest");

	_camera = new Camera(480, 270, 0, forest->width() * forest->tileWidth(), 0, forest->height() * forest->tileHeight(), _object);

   
	ServiceLocator::GetRenderer()->UseCamera(_camera);


	EventDispatcher::AddListener(_object, EventType::PLAYER_ENEMY_COLLIDED);
    return true;
}

bool PlayState::OnExit(){
	delete _region;
	delete _object;
	delete _camera;
	delete _infoMenu;

	ResourceManager::DeleteTexture("maya_running");
	ResourceManager::DeleteTexture("maya_standing");
	
    return true;
}
