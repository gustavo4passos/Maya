#include "../include/PlayState.h"

#include "../include/Button.h"
#include "../include/Door.h"
#include "../include/EventDispatcher.h"
#include "../include/Golem.h"
#include "../include/PushableObject.h"
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
	
	_maya = new Maya(200, 0);
	ServiceLocator::ProvidePlayer(_maya);

	_camera = new Camera(480, 270, 0, 0, 0, 0, _maya);
	ServiceLocator::GetRenderer()->UseCamera(_camera);
	_infoMenu = new InfoMenuGL3();

	_region = LevelLoader::ParseRegion("../res/regions/temple-area.region");	
	ServiceLocator::ProvideCurrentRegion(_region);

	SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("forest_sounds"), true);

	//_region->ChangeCurrentLevel("templeEntrance");
	//Level* forest = LevelLoader::ParseLevel("../res/levels/forest_2.tmx");
	//Level* mountain = LevelLoader::ParseLevel("../res/levels/mountain.tmx");
	//Level* templeEntrance = LevelLoader::ParseLevel("../res/levels/TempleEntrance1.tmx");
	//templeEntrance->AddGameObject(new PushableObject(CollisionRect(Rect(300, 0, 47, 41), CollisionBehavior::BLOCK), 47, 41));

	// _region->AddLevel(forest, "forest");
	// _region->AddLevel(mountain, "mountain");
	// _region->AddLevel(templeEntrance, "templeEntrance");

	//_region->ChangeCurrentLevel("forest");
	

	//Level* mountain = LevelLoader::ParseLevel("../res/levels/mountain.tmx");

	//_region->AddLevel(forest, "forest");
	//_region->AddLevel(mountain, "mountain");
	//_region->ChangeCurrentLevel("forest");

	SoundPlayer::PlayBGM(ResourceManager::GetMusic("piano-theme"), true);


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
