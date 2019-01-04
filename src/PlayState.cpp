#include "../include/PlayState.h"

#include "../include/EventDispatcher.h"
#include "../include/SaveSystem.h"
#include "../include/InfoMenuGL3.h"
#include "../include/InputModule.h"
#include "../include/LevelLoader.h"
#include "../include/Maya.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Region.h"

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

void PlayState::Render(Renderer* renderer, float deltaTime){
	//TODO(Gustavo): Below is a temporary solution for the camera position interpolation problem.
	// This solution must be integrated properly withing the camera code
	Vector2D pos = _maya->collisionRect().position();
	_maya->setPosition( pos.x() + _maya->velocity().x() * deltaTime,
		 				pos.y() + _maya->velocity().y() * deltaTime );
	_camera->Update();
	_maya->setPosition(pos.x(), pos.y());
	_region->RenderBackground(renderer, deltaTime);
	_maya->Draw(renderer, deltaTime);
	_region->RenderForeground(renderer, deltaTime);
	_infoMenu->Render(renderer);	
}

bool PlayState::OnEnter(){		
	
	Save* save = ServiceLocator::GetSaveSystem()->LoadGame();
	if(save == nullptr) return false;
	
	_maya = new Maya(save->playerPosition.x(), save->playerPosition.y());
	ServiceLocator::ProvidePlayer(_maya);

	_camera = new Camera(480, 270, 0, 0, 0, 0, _maya);
	ServiceLocator::GetRenderer()->UseCamera(_camera);
	_infoMenu = new InfoMenuGL3();

	_region = LevelLoader::ParseRegion(save->regionFilename);
	_region->ChangeCurrentLevel(save->subregionName);
	ServiceLocator::ProvideCurrentRegion(_region);

	SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("forest_sounds"), true);
	SoundPlayer::PlayBGM(ResourceManager::GetMusic("hello"), true);

	delete save;
    return true;
}

bool PlayState::OnExit(){
	_infoMenu->Clean();
	
	delete _region;
	delete _maya;
	delete _camera;
	delete _infoMenu;

	ResourceManager::DeleteTexture("maya_running");
	ResourceManager::DeleteTexture("maya_standing");

    return true;
}
