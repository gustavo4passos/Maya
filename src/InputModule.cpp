#include "../include/InputModule.h"

#include <iostream>

#include "../include/Game.h"
#include "../include/ErrorHandler.h"


InputModule::InputModule() {}

InputModule::~InputModule() {}

// ATRIBUTES INITIALIZATION

std::vector<InputModule::ButtonState> InputModule::_keyStates;

bool InputModule::_joysticksInit;
std::vector<SDL_Joystick*> InputModule::_joysticks;
std::vector< std::pair<Vector2D*, Vector2D*> > InputModule::_joystickValues;
std::vector< std::vector<InputModule::ButtonState> > InputModule::_joyButtonStates;
const int InputModule::_joyDeadZone;

std::vector<InputModule::ButtonState> InputModule::_mouseButtonStates;
Vector2D* InputModule::_mousePosition;




// MAIN FUNCTIONS

bool InputModule::Init()
{
	if(!SDL_WasInit(SDL_INIT_EVENTS))
	{
		if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
			std::cout << SDL_GetError() << "\n";
			return false;
		}
	}

	// Initializing mouse buttons
	for (int i = 0; i < 3; i++)
	{
		ButtonState temp;
		temp.isUp = false;
		temp.isDown = false;
		_mouseButtonStates.push_back(temp);
	}

	_mousePosition = new Vector2D(0, 0);

	// Initializing keyboard
	int a;
	int* numKeys = &a;
	const Uint8* tempKeyboard = SDL_GetKeyboardState(numKeys);
	if (numKeys == NULL) {
		LOG_ERROR(SDL_GetError());
		return false;
	}
		
	for (int i = 0; i < *numKeys; i++)
	{
		ButtonState temp;
		temp.isUp = false;
		temp.isDown = false;
		_keyStates.push_back(temp);
	}

	return true;
}

void InputModule::Update()
{

	// KEYBOARD

	// Update Keyboard
	//for (int i = 0; i < _keyStates.size(); i++)
	for (int i = 0; i < 10000; i++)
	{
		if (i >= _keyStates.size()) {
			int a = 23;
			bool c = false;
			ButtonState b = _keyStates[i%_keyStates.size()];
			continue;
		}
		const Uint8* tempSDLKeyboard = SDL_GetKeyboardState(0);
		if (tempSDLKeyboard[i] == 1) {
			_keyStates[i].isDown = true;
			_keyStates[i].isUp = false;
		}			
		else 
			if (_keyStates[i].isDown == true) {
				_keyStates[i].isUp = true;
				_keyStates[i].isDown = false;
			}
			else{
				_keyStates[i].isUp = false;
			}	
	}

	// Reset mouse buttons
	for (int i = 0; i < 3; i++)
	{
		_mouseButtonStates[i].isUp = false;
		_mouseButtonStates[i].isDown = false;
	}

	// Reset joystick buttons
	if (_joysticksInit)
	{
		for (int i = 0; i < _joyButtonStates.size(); i++)
		{
			for (int j = 0; j < _joyButtonStates[i].size(); j++)
			{
				_joyButtonStates[i][j].isUp = false;
				_joyButtonStates[i][j].isDown = false;
			}
		}
	}
	
	// Search for events (joystick and mouse)
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			Game::Instance()->clean();
			break;

		case SDL_JOYAXISMOTION:
			OnJoystickAxisMove(e);
			break;

		case SDL_JOYBUTTONDOWN:
			OnJoystickButtonDown(e);
			break;

		case SDL_JOYBUTTONUP:
			OnJoystickButtonUp(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			OnMouseButtonDown(e);
			break;

		case SDL_MOUSEBUTTONUP:
			OnMouseButtonUp(e);
			break;

		case SDL_MOUSEMOTION:
			OnMouseMove(e);
			break;

		}
	}	
}

void InputModule::Clean()
{
	if (_joysticksInit)
		for (unsigned int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(_joysticks[i]);
		}

	// delete _mousePosition;
}



// JOYSTICK

void InputModule::InitJoysticks()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
			LOG_ERROR(SDL_GetError());
			_joysticksInit = false;
		}			
	}

	if (SDL_NumJoysticks() <= 0)
		_joysticksInit = false;

	else
	{
		int numJoy = SDL_NumJoysticks();
		for (int i = 0; i < numJoy; i++)
		{
			SDL_Joystick *joy = SDL_JoystickOpen(i);
			if (joy == NULL) {
				LOG_ERROR(SDL_GetError());
			}							
				
			else 
			{
				_joysticks.push_back(joy);
				_joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

				std::vector<ButtonState> tempButtons;
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++)
				{
					ButtonState tempButtonState;
					tempButtonState.isUp = false;
					tempButtonState.isDown = false;
					tempButtons.push_back(tempButtonState);
				}
				_joyButtonStates.push_back(tempButtons);
				std::cout << "Joysticks has " << tempButtons.size() << " buttons\n";
			}
		}

		SDL_JoystickEventState(SDL_ENABLE);
		_joysticksInit = true;

		std::cout << "Initialized " << _joysticks.size() << " joystick(s)\n";
	}
}

bool InputModule::JoysticksInitialized()
{
	return _joysticksInit;
}

int InputModule::StickXValue(int joyIndex, JoyAnalogStick stick)
{
	if (_joystickValues.size() == 0) return 0;

	if (stick == LEFT_STICK)
		return _joystickValues[joyIndex].first->x();
	else
		return _joystickValues[joyIndex].second->x();
}

int InputModule::StickYValue(int joyIndex, JoyAnalogStick stick)
{
	if (_joystickValues.size() == 0) return 0;

	if (stick == LEFT_STICK)
		return _joystickValues[joyIndex].first->y();
	else
		return _joystickValues[joyIndex].second->y();
}

bool InputModule::IsJoyButtonUp(int joyIndex, int buttonNumber)
{
	return _joyButtonStates[joyIndex][buttonNumber].isUp;
}

bool InputModule::IsJoyButtonDown(int joyIndex, int buttonNumber)
{
	return _joyButtonStates[joyIndex][buttonNumber].isDown;
}

// handle _joysticks events
void InputModule::OnJoystickAxisMove(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;

	// LEFT stick moving in X axis
	if (e.jaxis.axis == 0)
	{
		if (e.jaxis.value > _joyDeadZone)
			_joystickValues[joyIndex].first->setX(1);

		else if (e.jaxis.value < -_joyDeadZone)
			_joystickValues[joyIndex].first->setX(-1);

		else
			_joystickValues[joyIndex].first->setX(0);
	}

	// LEFT stick moving in Y axis
	if (e.jaxis.axis == 1)
	{
		if (e.jaxis.value > _joyDeadZone)
			_joystickValues[joyIndex].first->setY(1);

		else if (e.jaxis.value < -_joyDeadZone)
			_joystickValues[joyIndex].first->setY(-1);

		else
			_joystickValues[joyIndex].first->setY(0);
	}

	// RIGHT stick moving in X axis
	if (e.jaxis.axis == 3)
	{
		if (e.jaxis.value > _joyDeadZone)
			_joystickValues[joyIndex].second->setX(1);

		else if (e.jaxis.value < -_joyDeadZone)
			_joystickValues[joyIndex].second->setX(-1);

		else
			_joystickValues[joyIndex].second->setX(0);
	}

	// RIGHT stick moving in Y axis
	if (e.jaxis.axis == 4)
	{
		if (e.jaxis.value > _joyDeadZone)
			_joystickValues[joyIndex].second->setY(1);

		else if (e.jaxis.value < -_joyDeadZone)
			_joystickValues[joyIndex].second->setY(-1);

		else
			_joystickValues[joyIndex].second->setY(0);
	}
}

void InputModule::OnJoystickButtonDown(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;
	_joyButtonStates[joyIndex][e.jbutton.button].isDown = true;
	_joyButtonStates[joyIndex][e.jbutton.button].isUp = false;
}

void InputModule::OnJoystickButtonUp(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;
	_joyButtonStates[joyIndex][e.jbutton.button].isDown = false;
	_joyButtonStates[joyIndex][e.jbutton.button].isDown = true;
}



// MOUSE

bool InputModule::IsMouseButtonUp(MouseButton button)
{
	return _mouseButtonStates[button].isUp;
}

bool InputModule::IsMouseButtonDown(MouseButton button)
{
	return _mouseButtonStates[button].isDown;
}

Vector2D * InputModule::mousePosition()
{
	return _mousePosition;
}

/*
void InputModule::ResetMouseButtons()
{
	for (int i = 0; i < 3; i++)
	{
		_mouseButtonStates.push_back(false);
	}
}
*/

// handle mouse events
void InputModule::OnMouseMove(SDL_Event &e)
{
	_mousePosition->setX(e.motion.x);
	_mousePosition->setY(e.motion.y);
}

void InputModule::OnMouseButtonDown(SDL_Event &e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		_mouseButtonStates[MOUSE_LB].isDown = true;
		break;

	case SDL_BUTTON_MIDDLE:
		_mouseButtonStates[MOUSE_MB].isDown = true;
		break;

	case SDL_BUTTON_RIGHT:
		_mouseButtonStates[MOUSE_RB].isDown = true;
		break;
	}
}

void InputModule::OnMouseButtonUp(SDL_Event &e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		_mouseButtonStates[MOUSE_LB].isUp = true;
		break;

	case SDL_BUTTON_MIDDLE:
		_mouseButtonStates[MOUSE_MB].isUp = true;
		break;

	case SDL_BUTTON_RIGHT:
		_mouseButtonStates[MOUSE_RB].isUp = true;
		break;
	}
}



// KEYBOARD

bool InputModule::isKeyDown(SDL_Scancode key)
{
	return _keyStates[key].isDown;	
}

bool InputModule::isKeyUp(SDL_Scancode key)
{
	return _keyStates[key].isUp;
}
