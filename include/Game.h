#ifndef GAME_H
#define GAME_H

#include "ErrorHandler.h"
#include "Window.h"
#include "Player.h"
#include "Level.h"
#include "Camera.h"

class Texture; 
class Mesh;
class InfoMenuGL3;

class Game {
public:
    Game() { }
    ~Game() { }

    bool Init();
    void Run();
    void Render(float deltaTime);
    void Update();
    void HandleEvents();

	void EndGameRequest();
    void ChangeResolution(int width, int height);
    void Clean();
    bool running() { return _running; }

private:
    Window* _window;
    Renderer* _renderer;
    unsigned int _lastFrame;
    bool _running;
};
#endif
