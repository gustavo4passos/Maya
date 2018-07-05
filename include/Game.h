#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "Window.h"
#include "Player.h"
#include "VertexArray.h"
#include "Camera.h"
#include "Level.h"

class Texture; 
class Mesh;
class InfoMenuGL3;

class Game {
public:
    Game() { }
    ~Game() { }

    bool Init();
    void Run();
    void Render(float positionFactor);
    void Update();
    void HandleEvents();

	void EndGameRequest();
    void Clean();
    bool running() { return _running; }

	friend class InfoMenuGL3;

private:
    Window* _window;
    Renderer* _renderer;

	Camera* _camera;
	GameObject* _object;
	InfoMenuGL3* _infoMenu;;
	Level* _level;
    Player* _maya;

    unsigned int _lastFrame;

    bool _running;
};
#endif
