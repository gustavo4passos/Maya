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
		IH_MOUSE_LB = 0,
		IH_MOUSE_MB = 1,
		IH_MOUSE_RB = 2
	}
	IH_MouseButton;

	typedef enum
	{
		IH_LEFT_STICK = 0,
		IH_RIGHT_STICK = 1
	}
	IH_Stick;
	
	// Main functions
	static bool Init();
	static void Update();
	static void Clean();

	// Joysticks
	static void InitJoysticks();
	static bool JoysticksInitialized();
	static bool JoyButtonState(int joyIndex, int buttonNumber);
	
	static int StickXValue(int joyIndex, IH_Stick);
	static int StickYValue(int joyIndex, IH_Stick);

	// Mouse
	static bool MouseButtonState(IH_MouseButton);
	static Vector2D *mousePosition();

	// Keyboard
	static bool isKeyPressed(SDL_Scancode);
	
private:
	
	InputModule();
	~InputModule();

	// Joystick atributes
	static bool _joysticksInit;
	static std::vector<SDL_Joystick*> _joysticks;
	static std::vector< std::pair<Vector2D*, Vector2D*> > _joystickValues;
	static std::vector< std::vector<bool> > _joyButtonStates;
	static const int _joyDeadZone = 20000;
	
	// Joystick methods
	static void OnJoystickAxisMove(SDL_Event&);
	static void OnJoystickButtonDown(SDL_Event&);
	static void OnJoystickButtonUp(SDL_Event&);


	// Mouse atributes
	static std::vector<bool> _mouseButtonStates;
	static Vector2D *_mousePosition;
	
	// Mouse methods
	static void OnMouseMove(SDL_Event&);
	static void OnMouseButtonDown(SDL_Event&);
	static void OnMouseButtonUp(SDL_Event&);


	// Keyboard atributes
	static const Uint8 *_keyboardState;
	
	// Keyboard methods
	static void OnKeyDown(SDL_Event&);
	static void OnKeyUp(SDL_Event&);
};

#endif