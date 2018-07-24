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
#include "../include/Maya.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_object->HandleInput();
	_infoMenu->HandleInput();

}

void PlayState::Update(){
	std::cout << "PLAYSTATE UPDATE\n";
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
	 if(!ResourceManager::LoadTexture("../res/assets/Maya_Run Sprite Sheet V03.png", "maya_running")) {
        LOG_ERROR("Unbale to load texture  \"Maya_Running\"");
		return false;
    }

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Standing.png", "maya_standing")){
	 	LOG_ERROR("Unable to load texture \"Maya_Standing\"");
		 return false;
	}

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Jump_V01.png", "maya_jumping")){
	 	LOG_ERROR("Unable to load texture \"Maya_Jumping\"");
		 return false;
	}

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Combat_Attack_v02.png", "maya_attacking")){
	 	LOG_ERROR("Unable to load texture \"Maya_Attacking\"");
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

	if(!ResourceManager::LoadSoundEffect("../res/audio/sfx/forest_sounds.mp3", "forest_sounds")){
		LOG_ERROR("Unable to load sound effect \"forest_sounds\"");
		return false;
	}

	if(!ResourceManager::LoadSoundEffect("../res/audio/sfx/dagger_swing.mp3", "dagger_swing")){
		LOG_ERROR("Unable to load sound effect \"dagger_swing\"");
		return false;
	}

	SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("forest_sounds"), true);
	
	_region = new Region();
	ServiceLocator::ProvideCurrentRegion(_region);
	
	CollisionRect mayaCollisionRect = CollisionRect(Rect(200, 0, 10, 30), CollisionBehavior::BLOCK, 12, 7);
	_object = new Maya(mayaCollisionRect, 36, 39);

	ServiceLocator::ProvidePlayer(_object);

	_infoMenu = new InfoMenuGL3();

	Level* forest = ResourceManager::ParseLevel("../res/levels/forest_2.tmx");
	forest->AddEnemy(new EvilSonic(CollisionRect(10, 100, 10, 30, 12, 5), 36, 39));
	
	forest->AddGameObject(new Button(CollisionRect(Rect(130, 430, 31, 22), CollisionBehavior::BLOCK, 1, 10), 32, 32, "forest-button-1", false));
	forest->AddGameObject(new Door(CollisionRect(Rect(384, 420, 32, 32), CollisionBehavior::IGNORE), 32, 32, "forest-button-1", false));

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
