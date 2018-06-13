#include "../include/InfoMenu.h"

#include <sstream>

#include "../include/imgui.h"
#include "../include/imgui_impl_sdl_gl3.h"

#include "../include/Game.h"
#include "../include/InputModule.h"
#include "../include/Level.h"

#define LOCAL_PERSIST static

InfoMenuGL3::InfoMenuGL3(Game* game, Window* window, Level* level, Player* player, GameObject* object) :
	_gameptr(game),
	_windowptr(window),
	_levelptr(level),
	_playerptr(player),
	_object(object),
	_showmenu(false),
	_currentMenu(NO_MENU),
	_showCollisionBoxes(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSdlGL3_Init(_windowptr->_windowHndl);
	ImGui::StyleColorsDark();

	_clearColor[0] = 0.f;
	_clearColor[1] = 1.f;
	_clearColor[2] = 0.f;
	_clearColor[3] = 1.f;
	
}	

InfoMenuGL3::~InfoMenuGL3() { }
void InfoMenuGL3::HandleInput(){
	if(InputModule::WasKeyReleased(InputModule::ESC)){
		if(_currentMenu == NO_MENU) _currentMenu = OPTIONS_MENU;
		else _currentMenu = NO_MENU;
	}
	if(InputModule::WasKeyReleased(InputModule::TAB)){
	  	_showmenu = !_showmenu;
	}
}

void InfoMenuGL3::Render(Renderer* renderer) {
	ImGui_ImplSdlGL3_NewFrame(_windowptr->_windowHndl);

	if(ImGui::BeginMainMenuBar()) {
		ImGui::Text("Maya position");
		ImGui::Value("X", _playerptr->position().x());
		ImGui::Value("Y", _playerptr->position().y());
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Value("FPS", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Spacing();

		if(ImGui::BeginMenu("Clear color")){
			ImGui::ColorEdit4("Color picker", _clearColor);
			renderer->SetClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
			ImGui::EndMenu();
		}


		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		if(ImGui::BeginMenu("Maya")){
			if(ImGui::BeginMenu("Position")){
				float posx = _playerptr->position().x();
				float posy = _playerptr->position().y();

				if(ImGui::InputFloat("X position", &posx)){
					_playerptr->setPosition(posx, posy);
				}
				if(ImGui::InputFloat("Y position", &posy)){
					_playerptr->setPosition(posx, posy);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if(ImGui::BeginMenu("Collision rects")){
			for(unsigned int i = 0; i < _levelptr->_collisionRects.size(); i++){
				std::stringstream objectname;
				objectname << "Object " << i;
				if(ImGui::BeginMenu(objectname.str().c_str())){
					float x, y;
					x = _levelptr->_collisionRects[i]->x();
					y = _levelptr->_collisionRects[i]->y();
					int w, h;
					w = _levelptr->_collisionRects[i]->w();
					h = _levelptr->_collisionRects[i]->h();
					
					if(ImGui::SliderFloat("X", &x, -400, 880)) {
						_levelptr->_collisionRects[i]->setX(x);
					}
					if(ImGui::SliderFloat("Y", &y, -400, 880)) {
						_levelptr->_collisionRects[i]->setY(y);
					}
					if(ImGui::SliderInt("Width", &w, 0, 900)) {
						_levelptr->_collisionRects[i]->_w = w;
					}
					if(ImGui::SliderInt("Height", &h, 0, 900)) {
						_levelptr->_collisionRects[i]->_h = h;
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndMenu();
		}

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
		ImGui::EndMainMenuBar();
	}

	if(_showmenu){ 
		ImGui::Begin("Grass");
		ImGui::Text("Position");
		static float x, y;
		x = _object->position().x();	
		y = _object->position().y();
		if(ImGui::SliderFloat("X", &x, 0, 480)){
			_object->setPosition(x, _object->position().y());
		}
		if(ImGui::SliderFloat("Y", &y, 0, 270)){
			_object->setPosition(_object->position().x(), y);
		}
		ImGui::Value("VelX", _object->velocity().x());
		ImGui::Value("VelY", _object->velocity().y());

		ImGui::End();
	}

	if(_currentMenu){
		int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
		ImVec2 size(200, 200);
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
					ImGui::PushItemWidth(100);
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

					ImGui::Dummy(ImVec2(20, 20));

					bool vsync = _windowptr->_vsync;
					if(ImGui::Checkbox("Vsync", &vsync)){
						_windowptr->SetVsync(vsync);
					}

					bool fullscreen = _windowptr->_fullscreen;
					if(ImGui::Checkbox("Fullscreen", &fullscreen)){
						_windowptr->SetFullscreen(fullscreen);
						renderer->SetViewportSize(_windowptr->width(), _windowptr->height());
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
			default:
			break;
		}
	}

	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}	

void InfoMenuGL3::RenderCollisionBoxes(Renderer* renderer){
	Rect rct = _object->collisionRect();
	Color red;
	red.r = 1.f;
	red.g = 0.f;
	red.b = 0.f;
	red.a = 0.4f;
	renderer->DrawFillRect(&rct, &red);

	for(std::vector<Rect*>::iterator it = _levelptr->_collisionRects.begin();  
	    it != _levelptr->_collisionRects.end(); ++it)
	{	  
		renderer->DrawFillRect(*it, &red);
	}

}
void InfoMenuGL3::Clean(){
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();
}	
