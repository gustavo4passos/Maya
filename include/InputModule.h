#ifndef __INPUT_MODULE__
#define __INPUT_MODULE__

#include "../include/Vector2D.h"
#include <SDL.h>
#include <vector>

class InputModule
{
public:	

	typedef enum
	{
		MOUSE_LB = 0,
		MOUSE_MB = 1,
		MOUSE_RB = 2
	}
	MouseButton;

	typedef enum
	{
		JOY_A,
		JOY_B,
		JOY_X,
		JOY_Y,
		JOY_LB,
		JOY_RB,
		JOY_BACK,
		JOY_START,
		JOY_LSTICK,
		JOY_RSTICK,		
		JOY_UP,
		JOY_DOWN,
		JOY_LEFT,
		JOY_RIGHT
	}
	JoyButton;

	typedef enum
	{
		LEFT_STICK = 0,
		RIGHT_STICK = 1
	}
	JoyAnalogStick;

	typedef enum
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	}
	Key;

	
	// Main functions
	static bool Init();
	static void Update();
	static void Clean();

	// Joysticks
	static bool JoysticksInitialized();
	static void InitJoysticks();	
	static bool IsJoyButtonUp(int joyIndex, int buttonNumber);
	static bool IsJoyButtonDown(int joyIndex, int buttonNumber);
	
	static int StickXValue(int joyIndex, JoyAnalogStick);
	static int StickYValue(int joyIndex, JoyAnalogStick);

	// Mouse
	static bool IsMouseButtonUp(MouseButton);
	static bool IsMouseButtonDown(MouseButton);
	static Vector2D *mousePosition();

	// Keyboard
	static bool isKeyDown(SDL_Scancode);
	static bool isKeyUp(SDL_Scancode);
	
private:
	
	InputModule();
	~InputModule();

	typedef struct buttonstate
	{
		bool isUp;
		bool isDown;
	}
	ButtonState;
	
	// Joystick atributes
	static bool _joysticksInit;
	static std::vector<SDL_Joystick*> _joysticks;
	static std::vector< std::pair<Vector2D*, Vector2D*> > _joystickValues;
	static const int _joyDeadZone = 15000;
	static std::vector< std::vector<ButtonState> > _joyButtonStates;
	
	
	// Joystick methods
	static void OnJoystickAxisMove(SDL_Event&);
	static void OnJoystickButtonDown(SDL_Event&);
	static void OnJoystickButtonUp(SDL_Event&);


	// Mouse atributes	
	static Vector2D *_mousePosition;
	static std::vector<ButtonState> _mouseButtonStates;
	
	// Mouse methods
	static void OnMouseMove(SDL_Event&);
	static void OnMouseButtonDown(SDL_Event&);
	static void OnMouseButtonUp(SDL_Event&);


	// Keyboard atributes
	static std::vector<ButtonState> _keyStates;

};

#endif