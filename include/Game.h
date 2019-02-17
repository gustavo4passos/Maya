#ifndef GAME_H
#define GAME_H

#include "SettingsManager.h"

#include "Window.h"
#include "Renderer.h"

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
    void Clean();
    bool running() const { return _running; }

    // Change settings
    void ChangeResolution(int width, int height);
    void SetVsync(bool active);
    void SetFullscreen(bool active);
    
private:
    Window* _window;
    Renderer* _renderer;
    SettingsManager _settingsManager;
    unsigned int _lastFrame;
    bool _running;
};
#endif
