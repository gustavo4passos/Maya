#include "../include/InfoMenuGL3.h"

#include <sstream>

#include "imgui_impl_sdl_gl3.h"

#include "../include/Enemy.h"
#include "../include/Event.h"
#include "../include/EventDispatcher.h"
#include "../include/Game.h"
#include "../include/GameStateMachine.h"
#include "../include/InputModule.h"
#include "../include/LevelLoader.h"
#include "../include/Maya.h"
#include "../include/PhysicsEngine.h"
#include "../include/Region.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"
#include "../include/SettingsManager.h"
#include "../include/SoundPlayer.h"

#define LOCAL_PERSIST static

InfoMenuGL3::InfoMenuGL3() :
	_showmenu(false),
	_currentMenu(NO_MENU),
	_showCollisionBoxes(false)
{
	_gameptr = ServiceLocator::GetGame(); 
	_windowptr = ServiceLocator::GetWindow();
	_player = ServiceLocator::GetPlayer();
	_levelptr = ServiceLocator::GetCurrentLevel();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetStyle().Alpha = 0.9f;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
	ImGui_ImplSdlGL3_Init(_windowptr->_windowHndl);
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiBackendFlags_HasGamepad;
	ImGui::StyleColorsDark();

	_clearColor[0] = 0.f;
	_clearColor[1] = 1.f;
	_clearColor[2] = 0.f;
	_clearColor[3] = 1.f;

	_masterVolume = SoundPlayer::MasterVolume();
	
	EventDispatcher::AddListener(this, EventType::LEVEL_CHANGED);
}	

InfoMenuGL3::~InfoMenuGL3() {
	EventDispatcher::RemoveListener(this, EventType::LEVEL_CHANGED);
}

void InfoMenuGL3::HandleInput(){
	if(InputModule::WasKeyReleased(InputModule::ESC)){
		if(_currentMenu == NO_MENU) _currentMenu = OPTIONS_MENU;
		else _currentMenu = NO_MENU;
	}
	if(InputModule::WasKeyReleased(InputModule::TAB)){
	  	_showmenu = !_showmenu;
	}
	if(InputModule::IsKeyPressed(InputModule::ENTER)){
		if(_currentMenu == OPTIONS_MENU){
			_gameptr->EndGameRequest();
		}
	}
}

void InfoMenuGL3::Render(Renderer* renderer) {
	ImGui_ImplSdlGL3_NewFrame(_windowptr->_windowHndl);

	RenderMenuBar(renderer);

	if(_showmenu){ 
		RenderGameObjectInfoMenu();
	}

	if(_currentMenu){
		int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
		ImVec2 size(200, 230);
		ImVec2 pos((_windowptr->width() - size.x) / 2, (_windowptr->height() - size.y) / 2);

		ImGui::SetNextWindowSize(size);
		ImGui::SetNextWindowPos(pos);

		switch(_currentMenu){
			case OPTIONS_MENU: 
			{
				if(ImGui::Begin("Settings", NULL, flags)){
					ImFont font = *ImGui::GetFont();
					font.FontSize = 20;
					ImGui::PushFont(&font);
					ImGui::Indent();
					ImGui::Indent();
					ImGui::Dummy(ImVec2(40, 20));
					ImGui::Spacing();

					if(ImGui::Button("Video Settings")){
						_currentMenu = VIDEO_MENU;
					}

					ImGui::Spacing();

					if(ImGui::Button("Sound Settings")) {
						_currentMenu = SOUND_MENU;
					}

					ImGui::PushItemWidth(100);
					ImGui::Spacing();
					if(ImGui::Button("Return to Game")){
						_currentMenu = NO_MENU;
					}

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					if(ImGui::Button("Quit game")){
						_gameptr->EndGameRequest();
					}
					ImGui::PopFont();	
					ImGui::End();
				}
			} break;
			case VIDEO_MENU: 
			{
				if(ImGui::Begin("Video Options", NULL, flags)){

					ImGui::Indent();
					ImGui::Indent();
					
					ImGui::Dummy(ImVec2(0, 10));

					std::stringstream resolution;
					resolution << "  Resolution\n   " << _windowptr->width() << "x" << _windowptr->height(); 
					ImGui::Text(resolution.str().c_str());

					if(!ServiceLocator::GetWindow()->IsFullscreen()) {
						if(ImGui::Button("Set resolution")) {
							_currentMenu = SET_RESOLUTION_MENU;
						}
					}
					ImGui::Dummy(ImVec2(20, 20));

					bool vsync = _windowptr->_vsync;
					if(ImGui::Checkbox("Vsync", &vsync)){
						ServiceLocator::GetGame()->SetVsync(vsync);
					}

					bool fullscreen = _windowptr->_fullscreen;
					if(ImGui::Checkbox("Fullscreen", &fullscreen)){
						ServiceLocator::GetGame()->SetFullscreen(fullscreen);
					}

					ImGui::Dummy(ImVec2(20, 20));
					ImGui::Dummy(ImVec2(20, 20));
					ImGui::SameLine();
					if(ImGui::Button("Cancel")){
					  	_currentMenu = OPTIONS_MENU;
					}

					ImGui::End();
				}
			} break;
			case SET_RESOLUTION_MENU:
			{
				size.x += 150;
				ImVec2 pos((_windowptr->width() - size.x) / 2, (_windowptr->height() - size.y) / 2);
				ImGui::SetNextWindowSize(size);
				ImGui::SetNextWindowPos(pos);

				// This menu shouldn't be available when in fullscreen mode
				// (Fullscreen is now implement by using a borderless fullscreen with the desktop dimensions)
				// So if the screen is fullscreenized while at the set resolution menu, return to the video menu
				if(ServiceLocator::GetWindow()->IsFullscreen()) _currentMenu = VIDEO_MENU;
				else {
					if(ImGui::Begin("Set Resolution", NULL, flags)) {
						ImGui::Indent();
						ImGui::Indent();
						ImGui::Dummy(ImVec2(0, 10));


						// Create resolution strings from available resolutions
						std::vector<std::string> resolutions;
						std::set<std::pair<int, int> > availableDisplayModes = ServiceLocator::GetWindow()->RetrieveDisplayModes();
						static int selectedItem = -1;
						for(auto displayMode : availableDisplayModes) {
							std::stringstream mode;
							mode << std::to_string(displayMode.first) << "x" << std::to_string(displayMode.second);
							resolutions.push_back(mode.str());
						}
								
						// Make a vector of null terminated strings for ImGui
						const char** nullTerminatedResolutionStrings = new const char * [resolutions.size()];

						for(unsigned i = 0; i < resolutions.size(); i++) {
							nullTerminatedResolutionStrings[i] = resolutions[i].c_str();
						}

						ImGui::ListBox("", &selectedItem, nullTerminatedResolutionStrings, resolutions.size());

						if(ImGui::Button("Apply")) {
							int count = 0;
							for(auto mode : availableDisplayModes) {
								if(count == selectedItem) {
									ServiceLocator::GetGame()->ChangeResolution(mode.first, mode.second);
									ImVec2 pos((_windowptr->width() - size.x) / 2, (_windowptr->height() - size.y) / 2);
									ImGui::SetNextWindowPos(pos);
									break; 
								}
								count++;
							}
						}
						if(ImGui::Button("Back")) {
							_currentMenu = VIDEO_MENU;
						} 

						delete[] nullTerminatedResolutionStrings;
						ImGui::End();
					}
				}
			} break;

			case SOUND_MENU: 
			{
				if(ImGui::Begin("Sound Settings", NULL, flags)) {
					ImGui::Indent();
					ImGui::Indent();

					ImGui::Dummy(ImVec2(0.f, 15.f));
					ImGui::Text("Master Volume");

					ImGui::Dummy(ImVec2(-10.f, 15.f));
					_masterVolume = SoundPlayer::MasterVolume() / 1.28;
					if(ImGui::SliderInt("", &_masterVolume, 0, 100))
					{
						SoundPlayer::SetMasterVolume(_masterVolume * 1.28);
					}

					ImGui::Dummy(ImVec2(0.f, 50.f));
					if(ImGui::Button("Return to Game")) {
						_currentMenu = NO_MENU;
					}

					ImGui::Dummy(ImVec2(20.f, 5.f));
					if(ImGui::Button("Back")) {
						_currentMenu = OPTIONS_MENU;
					}
					ImGui::End();
				}
			} break;

			default:
			break;
		}
	}

	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}	

void InfoMenuGL3::Clean(){
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();
}	

bool InfoMenuGL3::OnNotify(Event* event) {
	if(event->type() == EventType::LEVEL_CHANGED) {
		_levelptr = ServiceLocator::GetCurrentLevel();
		return false;
	}
	
	return false;
}

void InfoMenuGL3::RenderCollisionBoxes(Renderer* renderer){
	Rect mayaRct = _player->collisionRect();
	Rect weaponRct = dynamic_cast<Maya*>(_player)->weapon()->collisionRect();

	DrawCollisionBox(&mayaRct, renderer);
	DrawCollisionBox(&weaponRct, renderer);
	
	if(_levelptr != NULL) {
		for(std::vector<CollisionRect*>::iterator it = _levelptr->_collisionRects.begin();  
			it != _levelptr->_collisionRects.end(); ++it)
		{	  
			DrawCollisionBox(*it, renderer);
		}
		
		for(auto enemy = _levelptr->_enemies.begin(); enemy != _levelptr->_enemies.end(); enemy++){
			Rect enemyrct = (*enemy)->collisionRect();
			DrawCollisionBox(&enemyrct, renderer);

		}

		for(auto gameObject = _levelptr->_gameObjects.begin(); gameObject != _levelptr->_gameObjects.end(); gameObject++){
			Rect gameObjectRect = (*gameObject)->collisionRect();
			DrawCollisionBox(&gameObjectRect, renderer);
		}
	}
}

void InfoMenuGL3::RenderMenuBar(Renderer* renderer){
	if(ImGui::BeginMainMenuBar()) {
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Value("FPS", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Spacing();

		// if(ImGui::BeginMenu("Clear color")){
		// 	ImGui::ColorEdit4("Color picker", _clearColor);
		// 	renderer->SetClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
		// 	ImGui::EndMenu();
		// // }

		// ImGui::Spacing();
		// ImGui::Separator();
		// ImGui::Spacing();

	// if(_levelptr != NULL) {
	// 	if(ImGui::BeginMenu("Collision rects")){
	// 		for(unsigned int i = 0; i < _levelptr->_collisionRects.size(); i++){
	// 			std::stringstream objectname;
	// 			objectname << "Object " << i;
	// 			if(ImGui::BeginMenu(objectname.str().c_str())){
	// 				DrawCollisionBox(_levelptr->_collisionRects[i], renderer);
	// 				float x, y;
	// 				x = _levelptr->_collisionRects[i]->x();
	// 				y = _levelptr->_collisionRects[i]->y();
	// 				int w, h;
	// 				w = _levelptr->_collisionRects[i]->w();
	// 				h = _levelptr->_collisionRects[i]->h();
					
	// 				if(ImGui::SliderFloat("X", &x, -400, 880)) {
	// 					_levelptr->_collisionRects[i]->setX(x);
	// 				}
	// 				if(ImGui::SliderFloat("Y", &y, -400, 880)) {
	// 					_levelptr->_collisionRects[i]->setY(y);
	// 				}
	// 				if(ImGui::SliderInt("Width", &w, 0, 900)) {
	// 					_levelptr->_collisionRects[i]->setW(w);
	// 				}
	// 				if(ImGui::SliderInt("Height", &h, 0, 900)) {
	// 					_levelptr->_collisionRects[i]->setH(h);
	// 				}
	// 				ImGui::EndMenu();
	// 			}
	// 		}
	// 		ImGui::EndMenu();
	// 	}
	// }
	// else {
	// 	ImGui::Text("No active level");
	// }

		ImGui::Separator();

		if(ImGui::Button("Settings")){
		  	_currentMenu = OPTIONS_MENU;
		}
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Checkbox("Show collision boxes", &_showCollisionBoxes);
		if(_showCollisionBoxes){
		  	RenderCollisionBoxes(renderer);
		}
		ImGui::Separator();
	
		ImGui::Dummy(ImVec2(30.f, 0.f));
		ImGui::Text("Press tab to see player stats");
		ImGui::Dummy(ImVec2(30.f, 0.f));
		ImGui::Separator();

		ImGui::Dummy(ImVec2(10.f, 0.f));
		if(ImGui::BeginMenu("Add Subregion")){
			std::vector<std::string> levels = GetFilenamesInLevelsFolder();
			for(auto level = levels.begin(); level != levels.end(); level++){
				if((*level) != "." && (*level) != "..") {
					if(ImGui::Button((*level).c_str())){
						(*level).insert(0, "../res/levels/");
						Level* newSubregion = LevelLoader::ParseLevel((*level).c_str());
						if(newSubregion != nullptr) {
							ServiceLocator::GetCurrentRegion()->AddLevel(newSubregion, *level);
						}
						else {
							LOG_ERROR("Unable to load subregion: " + *level);
						}

					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::Dummy(ImVec2(10.f, 0.f));
		ImGui::Separator();
		
		ImGui::Dummy(ImVec2(10.f, 0.f));
		if(ServiceLocator::GetCurrentRegion() != nullptr) {
			if(ImGui::BeginMenu("Change Subregion")) {
				std::vector<std::string> subRegionList = ServiceLocator::GetCurrentRegion()->SubRegionList();

				for(auto subRegion = subRegionList.begin(); subRegion != subRegionList.end(); subRegion++) {
					if(*subRegion != ServiceLocator::GetCurrentRegion()->currentSubRegion()){
						if(ImGui::Button(subRegion->c_str())){
							ServiceLocator::GetCurrentRegion()->ChangeCurrentLevel(*subRegion);
						}
					}
				}

				ImGui::End();
			}
		}
		else {
			ImGui::Text("No active region");
		}

		ImGui::EndMainMenuBar();
	}
}

void InfoMenuGL3::RenderGameObjectInfoMenu(){
		ImGui::Begin("Maya");
		ImGui::Text("Position");

		LOCAL_PERSIST float x, y;
		x = _player->position().x();	
		y = _player->position().y();
		Level* currentLevel = ServiceLocator::GetCurrentLevel();
		float maxLevelPositionX = currentLevel->width() * currentLevel->tileWidth() - ServiceLocator::GetPlayer()->w();
		float maxLevelPositionY = currentLevel->height() * currentLevel->tileHeight() - ServiceLocator::GetPlayer()->w();
		if(ImGui::SliderFloat("X", &x, 0, maxLevelPositionX)){
			_player->setPosition(x, _player->position().y());
		}
		if(ImGui::SliderFloat("Y", &y, 0, maxLevelPositionY)){
			_player->setPosition(_player->position().x(), y);
		}

		LOCAL_PERSIST float speed = _player->_speed;
		if(ImGui::InputFloat("Speed", &speed)){
		  _player->_speed = speed;
		}

		LOCAL_PERSIST float impulse = _player->_impulse;
		if(ImGui::InputFloat("Impulse", &impulse)){
		  	_player->_impulse = impulse;
		}
		ImGui::Value("VelX", _player->velocity().x());
		ImGui::Value("VelY", _player->velocity().y());

		ImGui::Spacing();
		LOCAL_PERSIST float gravity = PhysicsEngine::_gravity.y();
		if(ImGui::InputFloat("Gravity", &gravity)){
		  	PhysicsEngine::_gravity.setY(gravity);
		}

		ImGui::End();
}

void InfoMenuGL3::DrawCollisionBox(Rect* rect, Renderer* renderer) {
	LOCAL_PERSIST Color red = { 1.f, 0.f, 0.f, 0.4f };
	renderer->DrawFillRect(rect, &red);
}

#ifdef _WIN32
#include <windows.h>

std::string InfoMenuGL3::OpenFileDialog(){
	char filename[255];
	OPENFILENAME c = { };
	c.lStructSize = sizeof(c);
	c.hInstance = NULL;
	c.lpstrFilter = "TMX Files (.tmx)\0*.tmx\0";
	c.lpstrFile = filename;
	c.lpstrFile[0] = '\0';
	c.lpstrTitle = "Select level file";
	c.nMaxFile = sizeof(filename);
	c.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	GetOpenFileName(&c);
	
	return std::string(filename);
}

std::vector<std::string> InfoMenuGL3::GetFilenamesInLevelsFolder() {
  WIN32_FIND_DATA data;
  std::vector<std::string> files;
  HANDLE hFind = FindFirstFile("..\\res\\levels\\*", &data);
  if(hFind != INVALID_HANDLE_VALUE){
		do {
			files.push_back(data.cFileName);
		} while(FindNextFile(hFind, &data));
		FindClose(hFind);
  }
  return files;
}

#else 

const char * InfoMenuGL3::OpenFileDialog() {
  return NULL;
}

std::vector<std::string> InfoMenuGL3::GetFilenamesInLevelsFolder() {
	std::vector<std::string> files;
	return files;
}

#endif
