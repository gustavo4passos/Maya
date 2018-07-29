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
#include "../include/Maya.h"
#include "../include/MovingPlatform.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/Region.h"
#include "../include/Renderer.h"
#include "../include/LevelLoader.h"

const std::string PlayState::_playID = "PLAY";

void PlayState::HandleInput(){
	_maya->HandleInput();
	_infoMenu->HandleInput();

}

void PlayState::Update(){
	std::cout << "PlayState update1\n";
	_region->Update();
	std::cout << "PlayState update2\n";
    _maya->Update();
	std::cout << "PlayState update3\n";
	_camera->Update();
	std::cout << "PlayState update4\n";
}

void PlayState::Render(Renderer* renderer, float deltatime){
	//TODO(Gustavo): Below is a temporary solution for the camera position interpolation problem.
	// This solution must be integrated properly withing the camera code

	std::cout << "PlayState render1\n";
	Vector2D pos = _maya->collisionRect().position();
	std::cout << "PlayState render2\n";
	_maya->setPosition(pos.x() + _maya->velocity().x() * deltatime, 
				  		 pos.y() + _maya->velocity().y() * deltatime); 
	std::cout << "PlayState render3\n";
	_camera->Update();
	std::cout << "PlayState render4\n";
	
	_maya->setPosition(pos.x(), pos.y()); 
	std::cout << "PlayState render5\n";
	
	_region->Render(renderer, deltatime);
	std::cout << "PlayState render6\n";
	_maya->Draw(renderer, deltatime);
	std::cout << "PlayState render7\n";
	_infoMenu->Render(renderer);
	std::cout << "PlayState render8\n";

	
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