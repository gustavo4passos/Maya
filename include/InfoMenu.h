#ifndef INFOMENUGL3_H
#define INFOMENUGL3_h

/* Needs a valid OpenGL context to run */
/* Uses SDL for messaging */

class Game;
class GameObject;
class Player;
class Renderer;
class Window;

class InfoMenuGL3 {
public:
	InfoMenuGL3(Game* game, Window* window, Player* player, GameObject* object);
	~InfoMenuGL3();

	void Update();
	void HandleInput();
	void Render(Renderer* renderer);
	void Clean();

private:
	Game* _gameptr;
	GameObject* _object;
	Player* _playerptr;
	Window* _windowptr;

	float _clearColor[4];
	bool _showmenu;
};

#endif
