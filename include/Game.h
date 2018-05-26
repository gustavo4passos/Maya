#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "Window.h"


class Game {
public:
    Game() { }
    ~Game() { }

    bool Init();
    void Run();
    void Render();
    void Update();
    void Clean();
    void HandleEvents();
    bool running() { return _running; }

private:
    
    Window* _window;
    Renderer* _renderer;


    bool _running;
};
#endif