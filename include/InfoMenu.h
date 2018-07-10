#ifndef INFOMENUGL3_H
#define INFOMENUGL3_h

/* A Menu UI useful for showing and editing game attributes in real time */
/* Needs a valid OpenGL context to run */
/* Uses SDL messaging system */

#include <string>
#include <vector>

#include <imgui.h>

class Camera;
class Game;
class Window;
class Level;
class GameObject;
class Player;
class Rect;
class Renderer;

class InfoMenuGL3 {
public:
	InfoMenuGL3(Game* game, Window* window, Level* level, Player* player, GameObject* object);
	~InfoMenuGL3();

	void Update();
	void HandleInput();
	void Render(Renderer* renderer);
	void Clean();

private:

	enum MenuType {
		NO_MENU,
		OPTIONS_MENU,
		VIDEO_MENU
	};

	Game* _gameptr;
	Window* _windowptr;
	Level* _levelptr;
	Player* _playerptr;
	GameObject* _object;
	Camera* _camera;

	float _clearColor[4];
	bool _showmenu;
	MenuType _currentMenu;
	bool _showCollisionBoxes;

	void RenderMenuBar(Renderer* renderer);
	void RenderCollisionBoxes(Renderer* renderer);
	void RenderGameObjectInfoMenu();
	void DrawCollisionBox(Rect* rect, Renderer* renderer);
	
	std::string OpenFileDialog();
	std::vector<std::string> GetFilenamesInLevelsFolder();
};

#endif
