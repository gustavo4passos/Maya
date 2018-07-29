#include "../include/PlayState.h"

#include "../include/Button.h"
#include "../include/Door.h"
#include "../include/EventDispatcher.h"
#include "../include/Golem.h"
#include "../include/GameSwitches.h"
#include "../include/GameStateMachine.h"
#include "../include/InfoMenu.h"
#include "../include/InputModule.h"
#include "../include/LevelLoader.h"
#include "../include/Maya.h"
#include "../include/MovingPlatform.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Region.h"
#include "../include/Renderer.h"
#include "../include/PlatformSwitch.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_maya->HandleInput();
	_infoMenu->HandleInput();

}

void PlayState::Update(){
	_region->Update();
    _maya->Update();
	_camera->Update();
}

void PlayState::Render(Renderer* renderer, float deltatime){
	//TODO(Gustavo): Below is a temporary solution for the camera position interpolation problem.
	// This solution must be integrated properly withing the camera code
	Vector2D pos = _maya->collisionRect().position();
	_maya->setPosition(pos.x() + _maya->velocity().x() * deltatime, 
				  		 pos.y() + _maya->velocity().y() * deltatime); 
	_camera->Update();
	
	_maya->setPosition(pos.x(), pos.y()); 
	
	_region->Render(renderer, deltatime);
	_maya->Draw(renderer, deltatime);
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

	if(!ResourceManager::LoadTexture("../res/assets/Maya_Combat Attack v003.png", "maya_attacking")){
	 	LOG_ERROR("Unable to load texture \"Maya_Attacking\"");
		return false;	
	}
	
	if(!ResourceManager::LoadTexture("../res/assets/Maya_Stand Arms.png", "maya_waiting")){
	 	LOG_ERROR("Unable to load texture \"Maya_Waiting\"");
		return false;
	}

	if(!ResourceManager::LoadTexture("../res/sprites/button-red.png", "button")) {
		LOG_ERROR("Unable to load texture \"button\"");
		return false;
	}

	if(!ResourceManager::LoadTexture("../res/sprites/moving_platform_stretched.png", "moving-platform")) {
		LOG_ERROR("Unable to load texture \"moving-platform\"");
		return false;
	}
	
	if(!ResourceManager::LoadTexture("../res/sprites/door.png", "door")) {
		LOG_ERROR("Unable to load texture \"Door\"");
		return false;
	}

	if(!ResourceManager::LoadTexture("../res/assets/static-golem.png", "../res/assets/static-golem.png")) {
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

	if(!ResourceManager::LoadSoundEffect("../res/audio/sfx/damage.mp3", "damage")){
		LOG_ERROR("Unable to load sound effect \"damage\"");
		return false;
	}

	if(!ResourceManager::LoadSoundEffect("../res/audio/sfx/button-press.wav", "button-press")) {
		LOG_ERROR("Unable to load sound effect \"button-press\"");
		return false;
	}
	if(!ResourceManager::LoadMusic("../res/audio/music/piano-theme-drums.mp3", "BGM")){
        LOG_ERROR("Unable to load music.");
        return false;
    }
	if(!ResourceManager::LoadTexture("../res/sprites/platform-switch.png", "platform-switch")) {
		LOG_ERROR("Unable to load texture \"platform-switch\"");
		return false;
	}
    
    SoundPlayer::PlayBGM(ResourceManager::GetMusic("BGM"), true);
	SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("forest_sounds"), true);
	
	_region = new Region();
	ServiceLocator::ProvideCurrentRegion(_region);
	
	_maya = new Maya(200, 0);
	ServiceLocator::ProvidePlayer(_maya);

	_infoMenu = new InfoMenuGL3();

	_camera = new Camera(480, 270, 0, 0, 0, 0, _maya);
	ServiceLocator::GetRenderer()->UseCamera(_camera);

	Level* forest = LevelLoader::ParseLevel("../res/levels/forest_2.tmx");
	forest->AddGameObject(_maya->weapon());
	forest->AddEnemy(new Golem(480,0));
	forest->AddGameObject(new Button(130, 440, "forest-button-1", false));
	forest->AddGameObject(new Door(CollisionRect(Rect(384, 420, 32, 32), CollisionBehavior::IGNORE), 32, 32, "forest-button-1", false));
	forest->AddGameObject(new MovingPlatform(Vector2D(150, 90), Vector2D(485, 90), true, "forest-button-1"));
	forest->AddGameObject(new PlatformSwitch(555, 310, "forest-button-1"));

	if(forest == NULL) return false;

	Level* mountain = LevelLoader::ParseLevel("../res/levels/mountain.tmx");

	_region->AddLevel(forest, "forest");
	_region->AddLevel(mountain, "mountain");
	_region->ChangeCurrentLevel("forest");


    return true;
}

bool PlayState::OnExit(){
	delete _region;
	delete _maya;
	delete _camera;
	delete _infoMenu;

	ResourceManager::DeleteTexture("maya_running");
	ResourceManager::DeleteTexture("maya_standing");
	
    return true;
}