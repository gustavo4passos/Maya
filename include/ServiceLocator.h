#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

class GameSwitches;
class Level;
class Renderer;

class ServiceLocator {
public:
    static GameSwitches* GetGameSwitches() { return _gameSwitches; }
    static Level* GetCurrentLevel() { return _currentLevel; }
    static Renderer* GetRenderer() { return _renderer; }

    static void ProvideRenderer(Renderer* renderer) { _renderer = renderer; }
    static void ProvideCurrentLevel(Level* currentLevel) { _currentLevel = currentLevel; }
    static void ProvideGameSwitches(GameSwitches* gameSwitches) { _gameSwitches = gameSwitches; }

private:
    static Level* _currentLevel;
    static Renderer* _renderer;
    static GameSwitches* _gameSwitches;
};

#endif