#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__

#include "../include/Vector2D.h"
#include <SDL.h>
#include <vector>
typedef enum
{
	IH_LEFT_STICK,
	IH_RIGHT_STICK
}
IH_Stick;

typedef enum
{
	IH_MOUSE_LB = 0,
	IH_MOUSE_MB = 1,
	IH_MOUSE_RB = 2
}
IH_MouseButton;

class InputHandler
{
public:		

	static InputHandler* Instance();
	
	bool init();
	void update();
	void clean();

	// Joysticks
	void initJoysticks();
	bool joysticksInitialized();
	bool joyButtonState(int joyIndex, int buttonNumber);
	int xValue(int joyIndex, IH_Stick);
	int yValue(int joyIndex, IH_Stick);

	// Mouse
	bool mouseButtonState(IH_MouseButton);
	Vector2D *mousePosition();

	// Keyboard
	bool isKeyPressed(SDL_Scancode);
	
private:

	static InputHandler *instance;
	
	InputHandler();
	~InputHandler();

	// Joystick atributes
	bool joysticksInit;
	std::vector<SDL_Joystick*> joysticks;
	std::vector< std::pair<Vector2D*, Vector2D*> > joystickValues;
	std::vector< std::vector<bool> > joyButtonStates;
	const int joyDeadZone = 20000;
	// Joystick methods
	void onJoystickAxisMove(SDL_Event&);
	void onJoystickButtonDown(SDL_Event&);
	void onJoystickButtonUp(SDL_Event&);


	// Mouse atributes
	std::vector<bool> mouseButtonStates;
	Vector2D *_mousePosition;
	// Mouse methods
	void onMouseMove(SDL_Event&);
	void onMouseButtonDown(SDL_Event&);
	void onMouseButtonUp(SDL_Event&);


	// Keyboard atributes
	const Uint8 *keyboardState;
	// Keyboard methods
	void onKeyDown(SDL_Event&);
	void onKeyUp(SDL_Event&);
};

#endif