#include "../include/InputModule.h"

#include <iostream>

#include "../include/Game.h"
#include "../include/ErrorHandler.h"


InputModule::InputModule() {}

InputModule::~InputModule() {}

// ATRIBUTES INITIALIZATION

bool InputModule::_closeWindow;

uint32_t InputModule::_bfPressed;
uint32_t InputModule::_bfReleased;
uint32_t InputModule::_bfWasPressed;
uint32_t InputModule::_bfFlags;

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

	// Initializing closeWindow


	// Initializing keyboard BitFields
	_bfPressed = 0;
	_bfReleased = 0;
	_bfWasPressed = 0;
	_bfFlags = 0xFFFFFFFF;


	// Initializing mouse buttons
	for (int i = 0; i < 3; i++)
	{
		ButtonState temp;
		temp.isUp = false;
		temp.isDown = false;
		_mouseButtonStates.push_back(temp);
	}

	_mousePosition = new Vector2D(0, 0);
	
	return true;
}

void InputModule::Update()
{
	// Reset released info and declares 
	_bfReleased = 0;
	uint32_t currentDown = 0;

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
	
	// Search for events
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{		
		switch (e.type)
		{
		case SDL_QUIT:
			
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

		case SDL_KEYDOWN:
			switch (e.key.keysym.scancode) 
			{
			
			case SDL_SCANCODE_UP:
				currentDown = currentDown | UP;
				// std::cout << "Up:" << std::hex << "0x" << currentDown << "\n";
				break;
			
			case SDL_SCANCODE_DOWN:
				currentDown = currentDown | DOWN;
				// std::cout << "Down:" << std::hex << "0x" << currentDown << "\n";
				break;
			
			case SDL_SCANCODE_LEFT:
				currentDown = currentDown | LEFT;
				// std::cout << "Left:" << std::hex << "0x" << currentDown << "\n";
				break;
			
			case SDL_SCANCODE_RIGHT:
				currentDown = currentDown | RIGHT;
				// std::cout << "Right:" << std::hex << "0x" << currentDown << "\n";
				break;
			
			case SDL_SCANCODE_SPACE:
				currentDown = currentDown | SPACE;
				break;
			
			case SDL_SCANCODE_RETURN:
				currentDown = currentDown | ENTER;
				break;
			
			case SDL_SCANCODE_ESCAPE:
				currentDown = currentDown | ESC;
				break;
			
			case SDL_SCANCODE_BACKSPACE:
				currentDown = currentDown | BACKSPACE;
				break;
			
			case SDL_SCANCODE_TAB:
				currentDown = currentDown | TAB;
				break;
			
			case SDL_SCANCODE_LCTRL:
				currentDown = currentDown | LCTRL;
				break;
			
			case SDL_SCANCODE_RCTRL:
				currentDown = currentDown | RCTRL;
				break;
			
			case SDL_SCANCODE_LALT:
				currentDown = currentDown | LALT;
				break;
			
			case SDL_SCANCODE_RALT:
				currentDown = currentDown | RALT;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.scancode) 
			{

			case SDL_SCANCODE_UP:
				_bfReleased = _bfReleased | UP;
				break;

			case SDL_SCANCODE_DOWN:
				_bfReleased = _bfReleased | DOWN;
				break;

			case SDL_SCANCODE_LEFT:
				_bfReleased = _bfReleased | LEFT;
				break;

			case SDL_SCANCODE_RIGHT:
				_bfReleased = _bfReleased | RIGHT;
				break;

			case SDL_SCANCODE_SPACE:
				_bfReleased = _bfReleased | SPACE;
				break;

			case SDL_SCANCODE_RETURN:
				_bfReleased = _bfReleased | ENTER;
				break;

			case SDL_SCANCODE_ESCAPE:
				_bfReleased = _bfReleased | ESC;
				break;

			case SDL_SCANCODE_BACKSPACE:
				_bfReleased = _bfReleased | BACKSPACE;
				break;

			case SDL_SCANCODE_TAB:
				_bfReleased = _bfReleased | TAB;
				break;

			case SDL_SCANCODE_LCTRL:
				_bfReleased = _bfReleased | LCTRL;
				break;

			case SDL_SCANCODE_RCTRL:
				_bfReleased = _bfReleased | RCTRL;
				break;

			case SDL_SCANCODE_LALT:
				_bfReleased = _bfReleased | LALT;
				break;

			case SDL_SCANCODE_RALT:
				_bfReleased = _bfReleased | RALT;
				break;

			default:
				break;
			}
			break;
		}
	}

	_bfPressed = (_bfPressed | currentDown) & ~(_bfReleased);
	_bfWasPressed = (_bfFlags & _bfPressed);
	_bfFlags = (~(_bfPressed) | _bfReleased);

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

void InputModule::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(NULL, x, y);
}

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

bool InputModule::IsKeyPressed(InputModule::BF_Key key) 
{
	
	if (_bfPressed & key) {
		return true;
	}
		
	return false;

}

bool InputModule::WasKeyReleased(InputModule::BF_Key key) 
{
	if (_bfReleased & key) {		
		return true;
	} 
	return false;
}

bool InputModule::WasKeyPressed(BF_Key key)
{
	if (_bfWasPressed & key) {
		return true;
	}		
	return false;
}

bool InputModule::CloseWindowRequest()
{
	return _closeWindow;
}