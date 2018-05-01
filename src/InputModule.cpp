#include "../include/InputModule.h"
#include "../include/Game.h"
#include <iostream>

InputModule::InputModule() {}

InputModule::~InputModule() {}

// ATRIBUTES INITIALIZATION

bool InputModule::											_joysticksInit;
std::vector<SDL_Joystick*> InputModule::					_joysticks;
std::vector< std::pair<Vector2D*, Vector2D*> > InputModule::_joystickValues;
std::vector< std::vector<bool> > InputModule::				_joyButtonStates;
const int InputModule::										_joyDeadZone;

std::vector<bool> InputModule::								_mouseButtonStates;
Vector2D* InputModule::										_mousePosition;

const Uint8* InputModule::									_keyboardState;


// MAIN FUNCTIONS

bool InputModule::Init()
{
	SDL_ClearError();
	if(!SDL_WasInit(SDL_INIT_EVENTS))
	{
		if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
			std::cout << SDL_GetError() << "\n";
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		_mouseButtonStates.push_back(false);
	}

	_mousePosition = new Vector2D(0, 0);

	return true;
}

void InputModule::Update()
{

	// KEYBOARD
	_keyboardState = SDL_GetKeyboardState(0);

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			// QUIT
		case SDL_QUIT:
			Game::Instance()->clean();
			break;

			// JOYSTICK
		case SDL_JOYAXISMOTION:
			OnJoystickAxisMove(e);
			break;

		case SDL_JOYBUTTONDOWN:
			OnJoystickButtonDown(e);
			break;

		case SDL_JOYBUTTONUP:
			OnJoystickButtonUp(e);
			break;

			// MOUSE
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
			OnKeyDown(e);
			break;		

		case SDL_KEYUP:
			OnKeyUp(e);
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
}



// JOYSTICK

void InputModule::InitJoysticks()
{
	SDL_ClearError();
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
			std::cout << SDL_GetError() << "\n";
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
			if (joy == NULL)
				std::cout << SDL_GetError() << "\n";
			else {
				_joysticks.push_back(joy);
				_joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

				std::vector<bool> tempButtons;
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++)
				{
					tempButtons.push_back(false);
				}
				_joyButtonStates.push_back(tempButtons);
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

int InputModule::StickXValue(int joyIndex, IH_Stick stick)
{
	if (_joystickValues.size() == 0) return 0;

	if (stick == IH_LEFT_STICK)
		return _joystickValues[joyIndex].first->x();
	else
		return _joystickValues[joyIndex].second->x();
}

int InputModule::StickYValue(int joyIndex, IH_Stick stick)
{
	if (_joystickValues.size() == 0) return 0;

	if (stick == IH_LEFT_STICK)
		return _joystickValues[joyIndex].first->y();
	else
		return _joystickValues[joyIndex].second->y();
}

bool InputModule::JoyButtonState(int joyIndex, int buttonNumber)
{
	return _joyButtonStates[joyIndex][buttonNumber];
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
	_joyButtonStates[joyIndex][e.jbutton.button] = true;
}

void InputModule::OnJoystickButtonUp(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;
	_joyButtonStates[joyIndex][e.jbutton.button] = false;
}



// MOUSE

bool InputModule::MouseButtonState(IH_MouseButton button)
{
	return _mouseButtonStates[button];
}

Vector2D * InputModule::mousePosition()
{
	return _mousePosition;
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
		_mouseButtonStates[IH_MOUSE_LB] = true;
		break;

	case SDL_BUTTON_MIDDLE:
		_mouseButtonStates[IH_MOUSE_MB] = true;
		break;

	case SDL_BUTTON_RIGHT:
		_mouseButtonStates[IH_MOUSE_RB] = true;
		break;
	}
}

void InputModule::OnMouseButtonUp(SDL_Event &e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		_mouseButtonStates[IH_MOUSE_LB] = false;
		break;

	case SDL_BUTTON_MIDDLE:
		_mouseButtonStates[IH_MOUSE_MB] = false;
		break;

	case SDL_BUTTON_RIGHT:
		_mouseButtonStates[IH_MOUSE_RB] = false;
		break;
	}
}



// KEYBOARD

bool InputModule::isKeyPressed(SDL_Scancode key)
{
	if (_keyboardState == NULL) return false;

	if (_keyboardState[key] == 1) return true;

	return false;

}

// handle keyboard events
void InputModule::OnKeyDown(SDL_Event &e)
{

}

void InputModule::OnKeyUp(SDL_Event &e)
{
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		Game::Instance()->clean();
		break;
	}
}
