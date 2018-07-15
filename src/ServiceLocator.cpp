#include "../include/ServiceLocator.h"

#include "../include/GameSwitches.h"
#include "../include/Level.h"
#include "../include/Renderer.h"

Game* ServiceLocator::_game = nullptr;
GameSwitches* ServiceLocator::_gameSwitches = nullptr;
GameObject* ServiceLocator::_player = nullptr;
Level* ServiceLocator::_currentLevel = nullptr;
Renderer* ServiceLocator::_renderer = nullptr;
Window* ServiceLocator::_window = nullptr;
Region* ServiceLocator::_currentRegion = nullptr;