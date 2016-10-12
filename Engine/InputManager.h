#pragma once

#include <vector>
#include <unordered_map>
#include <SDL.h>

#include "misc/Vec2.h"
#include "Controller.h"

class InputManager
{
public:
	InputManager();

	~InputManager();

	//Keyboard
	bool wasKeyPressed(SDL_Keycode key);

	bool isKeyHeld(SDL_Keycode key);

	bool wasKeyReleased(SDL_Keycode key);
	
	//Quick Shortcuts.

	/**
	 @brief	Quick shortcut function to check if the ctrl modifier key is held.
	
	 @return	true if it pressed, false if not.
	 */
	bool ctrl();

	/**
	 @brief	Quick shortcut function to check if the alt modifier key is held.
	
	 @return	true if it pressed, false if not.
	 */

	bool alt();

	/**
	 @brief	Quick shortcut function to check if the shift modifier key is held.
	
	 @return	true if it pressed, false if not.
	 */
	bool shift();


	//Mouse
	/**
	@brief Was mouse button pressed.
	One of SDL_BUTTON_LEFT/MIDDLE/RIGHT/X1/X2

	@param button The mouse button to check.
	
	@return true if pressed, false if not.
	 */
	bool wasMouseButtonPressed(uint8_t button);

	/**
	@brief Is mouse button held.
	One of SDL_BUTTON_LEFT/MIDDLE/RIGHT/X1/X2

	@param button The mouse button to check.

	@return true if held, false if not.
	*/
	bool isMouseButtonHeld(uint8_t button);

	/**
	@brief Was mouse button released.
	One of SDL_BUTTON_LEFT/MIDDLE/RIGHT/X1/X2

	@param button The mouse button to check.

	@return true if released, false if not.
	*/
	bool wasMouseButtonReleased(uint8_t button);

	//Mouse Vectors

	/**
	 @fn	Vec2 InputManager::getMousePos();
	
	 @brief	Gets mouse position.
	
	 @author	Richard
	 @date	01/10/2016
	
	 @return	The mouse position.
	 */

	Vec2 getMousePos();

	Vec2 getMouseDirection();

	Vec2 getMouseWheelDirection();


	// Game Controllers
	bool wasControllerButtonPressed(int controller, Controller::Button button);

	bool isControllerButtonHeld(int controller, Controller::Button button);

	bool wasControllerButtonReleased(int controller, Controller::Button button);

	float getControllerAxis1D(int controller, Controller::Axis1D axis);

	Vec2 getControllerAxis2D(int controller, Controller::Axis2D axis);

	void playControllerRumble(int controller, float strength, uint32_t lengthMS);

	void stopControllerRumble(int controller);

	uint8_t getNumControllers();


	//Process Events
	void processKeyEvent(SDL_Event& e);

	void processMouseEvent(SDL_Event& e);

	void processGameControllerEvent(SDL_Event& e);

	void update();

private:

	enum ButtonState {
		Pressed,
		Held,
		Released,
		None,
		Unknown
	};

	std::unordered_map<SDL_Keycode, ButtonState> keys;

	std::unordered_map<uint8_t, ButtonState> mouseButtons;

	//Switch to standard array
	std::vector<Controller*> gamepads;

	const unsigned int MAX_GAMEPADS = 4;

	//Mouse Vectors
	Vec2 mousePos;

	Vec2 mouseDirection;

	Vec2 mouseWheelDirection;

	
	bool isGamepadValid(int controller);

	SDL_JoystickID getJoystickInstanceIDFromEvent(SDL_Event& e);

	void addController(SDL_Event& e);

	void addNewController(int joystickID, int arrayPos);

	void removeController(SDL_Event& e);
};