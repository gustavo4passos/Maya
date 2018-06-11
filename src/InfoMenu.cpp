#include "../include/InfoMenu.h"

#include "../include/imgui.h"
#include "../include/imgui_impl_sdl_gl3.h"

#include "../include/Game.h"
#include "../include/GameObject.h"
#include "../include/InputModule.h"

#define LOCAL_PERSIST static

InfoMenuGL3::InfoMenuGL3(Game* game, Window* window, Player* player, GameObject* object) :
	_gameptr(game),
	_object(object),
	_playerptr(player),
	_windowptr(window),
	_showmenu(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSdlGL3_Init(_windowptr->_windowHndl);
	ImGui::StyleColorsDark();

	_clearColor[0] = 0.f;
	_clearColor[1] = 0.f;
	_clearColor[2] = 1.f;
	_clearColor[3] = 1.f;
	
}	

InfoMenuGL3::~InfoMenuGL3() { }
void InfoMenuGL3::HandleInput(){
	if(InputModule::WasKeyPressed(InputModule::TAB)){
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

		bool vsync = _windowptr->_vsync;
		if(ImGui::Checkbox("Vsync", &vsync)){
			_windowptr->SetVsync(vsync);
		}

		bool fullscreen = _windowptr->_fullscreen;
		if(ImGui::Checkbox("Fullscreen", &fullscreen)){
			_windowptr->SetFullscreen(fullscreen);
			renderer->SetViewportSize(_windowptr->width(), _windowptr->height());
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

	ImGui::Render();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}	

void InfoMenuGL3::Clean(){
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();
}
	
