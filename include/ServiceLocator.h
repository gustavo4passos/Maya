#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

class AnimationPlayer;
class Game;
class GameObject;
class GameSwitches;
class Level;
class Region;
class Renderer;
class SaveSystem;
class SettingsManager;
class Window;

class ServiceLocator {
public:
    static AnimationPlayer* GetAnimationPlayer();
    static Game* GetGame();
    static GameObject* GetPlayer();
    static GameSwitches* GetGameSwitches();
    static Level* GetCurrentLevel();
    static Region* GetCurrentRegion();
    static Renderer* GetRenderer();
    static SaveSystem* GetSaveSystem();
    static SettingsManager* GetSettingsManager();
    static Window* GetWindow();

    static void ProvideAnimationPlayer(AnimationPlayer* animationPlayer) { _animationPlayer = animationPlayer; }
    static void ProvideCurrentLevel(Level* currentLevel) { _currentLevel = currentLevel; }
    static void ProvideCurrentRegion(Region* currentRegion) { _currentRegion = currentRegion; }
    static void ProvideGame(Game* game) { _game = game; }
    static void ProvideGameSwitches(GameSwitches* gameSwitches) { _gameSwitches = gameSwitches; }
    static void ProvidePlayer(GameObject* player) { _player = player; }
    static void ProvideRenderer(Renderer* renderer) { _renderer = renderer; }
    static void ProvideSaveSystem(SaveSystem* saveSystem) { _saveSystem = saveSystem; }
    static void ProvideSettingsManager(SettingsManager* settingsManager) { _settingsManager = settingsManager; }
    static void ProvideWindow(Window* window) { _window = window; }

private:
    static AnimationPlayer* _animationPlayer;
    static Game* _game;
    static GameObject* _player;
    static GameSwitches* _gameSwitches;
    static Level* _currentLevel;
    static Region* _currentRegion;
    static Renderer* _renderer;
    static SaveSystem* _saveSystem;
    static SettingsManager* _settingsManager;
    static Window* _window;
};

#endif