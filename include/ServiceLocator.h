#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

class Game;
class GameObject;
class GameSwitches;
class Level;
class Region;
class Renderer;
class Window;

class ServiceLocator {
public:
    static Game* GetGame();
    static GameObject* GetPlayer();
    static GameSwitches* GetGameSwitches();
    static Level* GetCurrentLevel();
    static Region* GetCurrentRegion();
    static Renderer* GetRenderer();
    static Window* GetWindow();

    static void ProvideCurrentLevel(Level* currentLevel) { _currentLevel = currentLevel; }
    static void ProvideGame(Game* game) { _game = game; }
    static void ProvideGameSwitches(GameSwitches* gameSwitches) { _gameSwitches = gameSwitches; }
    static void ProvidePlayer(GameObject* player) { _player = player; }
    static void ProvideCurrentRegion(Region* currentRegion) { _currentRegion = currentRegion; }
    static void ProvideRenderer(Renderer* renderer) { _renderer = renderer; }
    static void ProvideWindow(Window* window) { _window = window; }

private:
    static Game* _game;
    static GameObject* _player;
    static GameSwitches* _gameSwitches;
    static Level* _currentLevel;
    static Region* _currentRegion;
    static Renderer* _renderer;
    static Window* _window;
};

#endif