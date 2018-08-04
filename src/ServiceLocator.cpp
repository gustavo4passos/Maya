#include "../include/ServiceLocator.h"

#include "../include/GameSwitches.h"
#include "../include/Level.h"
#include "../include/Renderer.h"

Level* ServiceLocator::_currentLevel = nullptr;
Region* ServiceLocator::_currentRegion = nullptr;
Game* ServiceLocator::_game = nullptr;
GameSwitches* ServiceLocator::_gameSwitches = nullptr;
GameObject* ServiceLocator::_player = nullptr;
Renderer* ServiceLocator::_renderer = nullptr;
SaveSystem* ServiceLocator::_saveSystem = nullptr;
Window* ServiceLocator::_window = nullptr;

Game* ServiceLocator::GetGame() { 
    if(_game == nullptr) {
        LOG_ERROR("Game service has not been provided. Returning nullptr."); 
    }
    return _game; 
}

GameObject* ServiceLocator::GetPlayer() {
    if(_player == nullptr) {
        LOG_ERROR("Player service has not been provided. Returning nullptr."); 
    }
    return _player; 
}

GameSwitches* ServiceLocator::GetGameSwitches() {
    if(_gameSwitches == nullptr) {
        LOG_ERROR("GameSwitches service has not been provided. Returning nullptr."); 
    }
    return _gameSwitches; 
}

Level* ServiceLocator::GetCurrentLevel() {
    if(_currentLevel == nullptr) {
        LOG_ERROR("Level service has not been provided. Returning nullptr.");
    }
    return _currentLevel; 
}

Region* ServiceLocator::GetCurrentRegion() {
    if(_currentRegion == nullptr) {
        LOG_ERROR("Region service has not been provided. Returning nullptr."); 
    }
    return _currentRegion; 
}

Renderer* ServiceLocator::GetRenderer() {
    if(_renderer == nullptr) {
        LOG_ERROR("Renderer service has not been provided. Returning nullptr."); 
    }
    return _renderer;
}

SaveSystem* ServiceLocator::GetSaveSystem() {
    if(_renderer == nullptr) {
        LOG_ERROR("Save System service has not been provided. Returning nullptr.");
    }
    return _saveSystem;
}

Window* ServiceLocator::GetWindow() {
    if(_window == nullptr) {
        LOG_ERROR("Window service has not been provided. Returning nullptr."); 
    }
    return _window; 
}
