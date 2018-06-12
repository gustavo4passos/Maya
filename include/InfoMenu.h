#ifndef INFOMENUGL3_H
#define INFOMENUGL3_h

/* Needs a valid OpenGL context to run */
/* Uses SDL for messaging */

class Game;
class Window;
class Level;
class GameObject;
class Player;
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

	float _clearColor[4];
	bool _showmenu;
	MenuType _currentMenu;
	bool _showCollisionBoxes;

	void RenderCollisionBoxes(Renderer* renderer);
};

#endif
