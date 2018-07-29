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
	if(!ResourceManager::LoadTexture("../res/sprites/moving_platform_stretched.png", "moving-platform")) {
		LOG_ERROR("Unable to load texture \"moving-platform\"");
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

	if(!ResourceManager::LoadSoundEffect("../res/audio/sfx/button-press.wav", "button-press")) {
		LOG_ERROR("Unable to load sound effect \"button-press\"");
		return false;
	}
	if(!ResourceManager::LoadTexture("../res/sprites/platform-switch.png", "platform-switch")) {
		LOG_ERROR("Unable to load texture \"platform-switch\"");
		return false;
	}

	_maya = new Maya(200, 0);
	ServiceLocator::ProvidePlayer(_maya);

	_infoMenu = new InfoMenuGL3();

	_camera = new Camera(480, 270, 0, 0, 0, 0, _maya);
	ServiceLocator::GetRenderer()->UseCamera(_camera);

	_region = LevelLoader::ParseRegion("../res/regions/temple-area.region");
	ServiceLocator::ProvideCurrentRegion(_region);

	SoundPlayer::PlayBGM(ResourceManager::GetMusic("piano-theme"), true);
	SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("forest_sounds"), true);

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
