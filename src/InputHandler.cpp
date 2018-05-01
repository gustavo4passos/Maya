#include "../include/InputHandler.h"
#include "../include/Game.h"
#include <iostream>

// CLASS MANAGEMENT

InputHandler* InputHandler::instance = NULL;

InputHandler* InputHandler::Instance()
{
	if (instance == NULL) instance = new InputHandler;
	return instance;
}

InputHandler::InputHandler()
{
	for (int i = 0; i < 3; i++)
	{
		this->mouseButtonStates.push_back(false);
	}

	this->_mousePosition = new Vector2D(0, 0);
}

InputHandler::~InputHandler(){}



// MAIN FUNCTIONS

bool InputHandler::init()
{
	SDL_ClearError();
	if(!SDL_WasInit(SDL_INIT_EVENTS))
	{
		if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
			std::cout << SDL_GetError() << "\n";
			return false;
		}
	}
	return true;
}

void InputHandler::update()
{

	// KEYBOARD
	this->keyboardState = SDL_GetKeyboardState(0);

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
			this->onJoystickAxisMove(e);
			break;

		case SDL_JOYBUTTONDOWN:
			this->onJoystickButtonDown(e);
			break;

		case SDL_JOYBUTTONUP:
			this->onJoystickButtonUp(e);
			break;

			// MOUSE
		case SDL_MOUSEBUTTONDOWN:
			this->onMouseButtonDown(e);
			break;

		case SDL_MOUSEBUTTONUP:
			this->onMouseButtonUp(e);
			break;

		case SDL_MOUSEMOTION:
			this->onMouseMove(e);
			break;

		case SDL_KEYDOWN:
			this->onKeyDown(e);
			break;		

		case SDL_KEYUP:
			this->onKeyUp(e);
			break;
		}

	}


}

void InputHandler::clean()
{
	if (this->joysticksInit)
		for (unsigned int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(this->joysticks[i]);
		}
}



// JOYSTICK

void InputHandler::initJoysticks()
{
	SDL_ClearError();
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
			std::cout << SDL_GetError() << "\n";
			this->joysticksInit = false;
		}			
	}

	if (SDL_NumJoysticks() <= 0)
		this->joysticksInit = false;

	else
	{
		int numJoy = SDL_NumJoysticks();
		for (int i = 0; i < numJoy; i++)
		{
			SDL_Joystick *joy = SDL_JoystickOpen(i);
			if (joy == NULL)
				std::cout << SDL_GetError() << "\n";
			else {
				this->joysticks.push_back(joy);
				this->joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

				std::vector<bool> tempButtons;
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++)
				{
					tempButtons.push_back(false);
				}
				this->joyButtonStates.push_back(tempButtons);
			}
		}

		SDL_JoystickEventState(SDL_ENABLE);
		this->joysticksInit = true;

		std::cout << "Initialized " << joysticks.size() << " joystick(s)\n";
	}
}

bool InputHandler::joysticksInitialized()
{
	return this->joysticksInit;
}

int InputHandler::xValue(int joyIndex, IH_Stick stick)
{
	if (this->joystickValues.size() == 0) return 0;

	if (stick == IH_LEFT_STICK)
		return this->joystickValues[joyIndex].first->x();
	else
		return this->joystickValues[joyIndex].second->x();
}

int InputHandler::yValue(int joyIndex, IH_Stick stick)
{
	if (this->joystickValues.size() == 0) return 0;

	if (stick == IH_LEFT_STICK)
		return this->joystickValues[joyIndex].first->y();
	else
		return this->joystickValues[joyIndex].second->y();
}

bool InputHandler::joyButtonState(int joyIndex, int buttonNumber)
{
	return this->joyButtonStates[joyIndex][buttonNumber];
}

// handle joysticks events
void InputHandler::onJoystickAxisMove(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;

	// LEFT stick moving in X axis
	if (e.jaxis.axis == 0)
	{
		if (e.jaxis.value > this->joyDeadZone)
			this->joystickValues[joyIndex].first->setX(1);

		else if (e.jaxis.value < -this->joyDeadZone)
			this->joystickValues[joyIndex].first->setX(-1);

		else
			this->joystickValues[joyIndex].first->setX(0);
	}

	// LEFT stick moving in Y axis
	if (e.jaxis.axis == 1)
	{
		if (e.jaxis.value > this->joyDeadZone)
			this->joystickValues[joyIndex].first->setY(1);

		else if (e.jaxis.value < -this->joyDeadZone)
			this->joystickValues[joyIndex].first->setY(-1);

		else
			this->joystickValues[joyIndex].first->setY(0);
	}

	// RIGHT stick moving in X axis
	if (e.jaxis.axis == 3)
	{
		if (e.jaxis.value > this->joyDeadZone)
			this->joystickValues[joyIndex].second->setX(1);

		else if (e.jaxis.value < -this->joyDeadZone)
			this->joystickValues[joyIndex].second->setX(-1);

		else
			this->joystickValues[joyIndex].second->setX(0);
	}

	// RIGHT stick moving in Y axis
	if (e.jaxis.axis == 4)
	{
		if (e.jaxis.value > this->joyDeadZone)
			this->joystickValues[joyIndex].second->setY(1);

		else if (e.jaxis.value < -this->joyDeadZone)
			this->joystickValues[joyIndex].second->setY(-1);

		else
			this->joystickValues[joyIndex].second->setY(0);
	}
}

void InputHandler::onJoystickButtonDown(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;
	this->joyButtonStates[joyIndex][e.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event &e)
{
	int joyIndex = e.jaxis.which;
	this->joyButtonStates[joyIndex][e.jbutton.button] = false;
}



// MOUSE

bool InputHandler::mouseButtonState(IH_MouseButton button)
{
	return this->mouseButtonStates[button];
}

Vector2D * InputHandler::mousePosition()
{
	return this->_mousePosition;
}

// handle mouse events
void InputHandler::onMouseMove(SDL_Event &e)
{
	this->_mousePosition->setX(e.motion.x);
	this->_mousePosition->setY(e.motion.y);
}

void InputHandler::onMouseButtonDown(SDL_Event &e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		this->mouseButtonStates[IH_MOUSE_LB] = true;
		break;

	case SDL_BUTTON_MIDDLE:
		this->mouseButtonStates[IH_MOUSE_MB] = true;
		break;

	case SDL_BUTTON_RIGHT:
		this->mouseButtonStates[IH_MOUSE_RB] = true;
		break;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event &e)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		this->mouseButtonStates[IH_MOUSE_LB] = false;
		break;

	case SDL_BUTTON_MIDDLE:
		this->mouseButtonStates[IH_MOUSE_MB] = false;
		break;

	case SDL_BUTTON_RIGHT:
		this->mouseButtonStates[IH_MOUSE_RB] = false;
		break;
	}
}



// KEYBOARD

bool InputHandler::isKeyPressed(SDL_Scancode key)
{
	if (this->keyboardState == NULL) return false;

	if (this->keyboardState[key] == 1) return true;

	return false;

}

// handle keyboard events
void InputHandler::onKeyDown(SDL_Event &e)
{

}

void InputHandler::onKeyUp(SDL_Event &e)
{
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		Game::Instance()->clean();
		break;
	}
}
