#ifndef INFOMENUGL3_H
#define INFOMENUGL3_h

/* A Menu UI useful for showing and editing game attributes in real time */
/* Needs a valid OpenGL context to run */
/* Uses SDL messaging system */

#include <string>
#include <vector>

#include <imgui.h>

#include "GuiMenu.h"

class Camera;
class Game;
class Window;
class Level;
class GameObject;
class Player;
class Rect;
class Renderer;

class InfoMenuGL3 : public GuiMenu {
public:
	InfoMenuGL3();
	~InfoMenuGL3();

	void Update();
	void HandleInput();
	void Render(Renderer* renderer);
	void Clean();

	bool OnNotify(Event* event) override;

private:

	enum MenuType {
		NO_MENU,
		OPTIONS_MENU,
		VIDEO_MENU,
		SOUND_MENU,
		SET_RESOLUTION_MENU
	};

	Game* _gameptr;
	Window* _windowptr;
	Level* _levelptr;
	GameObject* _player;
	Camera* _camera;

	float _clearColor[4];
	bool _showmenu;
	MenuType _currentMenu;
	bool _showCollisionBoxes;

	// Audio
	int _masterVolume;

	void RenderMenuBar(Renderer* renderer);
	void RenderCollisionBoxes(Renderer* renderer);
	void RenderGameObjectInfoMenu();
	void DrawCollisionBox(Rect* rect, Renderer* renderer);
	
	std::string OpenFileDialog();
	std::vector<std::string> GetFilenamesInLevelsFolder();
};

#endif
