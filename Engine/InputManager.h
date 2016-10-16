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
	 @brief	Gets mouse position on the screen.
	
	 @return	The mouse position.
	 */
	Vec2 getMousePos();

	/**
	 @brief	Gets mouse direction vec. The movement vector since the last mouse movement.
	
	 @return	The mouse direction.
	 */
	Vec2 getMouseDirection();

	/**
	 @brief	Gets mouse wheel direction.
	
	 @todo Give range of output in the docs
	 @return	The mouse wheel direction.
	 */
	Vec2 getMouseWheelDirection();


	// Game Controllers
	
	/**
	 @brief	Was the specified controller button pressed. 
	
	 @param	controller	The controller index.
	 @param	button	  	The button enum.
	
	 @return	true if pressed, false if not or controller doesn't exist.
	 */
	bool wasControllerButtonPressed(int controller, Controller::Button button);

	/**
	@brief	Is the specified controller button held.

	@param	controller	The controller index.
	@param	button	  	The button enum.

	@return	true if held, false if not or controller doesn't exist.
	*/
	bool isControllerButtonHeld(int controller, Controller::Button button);

	/**
	@brief	Was the specified controller button released.

	@param	controller	The controller index.
	@param	button	  	The button enum.

	@return	true if released, false if not or controller doesn't exist.
	*/
	bool wasControllerButtonReleased(int controller, Controller::Button button);

	/**
	 @brief	Gets the controller's requested 1 dimensional axis (Triggers).
	
	 @param	controller	The controller index.
	 @param	axis	  	The controller axis.
	
	 @return	Float containing the controller's axis position.
	 */
	float getControllerAxis1D(int controller, Controller::Axis1D axis);

	/**
	@brief	Gets the controller's requested axis 2 dimensional (Triggers).

	@param	controller	The controller index.
	@param	axis	  	The controller axis.

	@return	Vec2 containing the controller's axis position.
	*/
	Vec2 getControllerAxis2D(int controller, Controller::Axis2D axis);

	/**
	 @brief	Play a rumble effect on the specified controller.
	
	 @param	controller	The controller index.
	 @param	strength  	The strength of the rumble. Between 0-1.
	 @param	lengthMS  	The length in milliseconds.
	 */
	void playControllerRumble(int controller, float strength, uint32_t lengthMS);

	/**
	 @brief	Stops a currently active rumble effect on the specified controller.
	
	 @param	controller	The controller index.
	 */
	void stopControllerRumble(int controller);

	/**
	 @brief	Gets number of controllers detected.
	
	 @return	The number of controllers.
	 */
	uint8_t getNumControllers();


	//Process Events

	/**
	 @brief	Process SDL Keyboard event into internal arrays.
	
	 @param [in,out]	e	The keyboard SDL_Event to process.
	 */
	void processKeyEvent(SDL_Event& e);

	/**
	@brief	Process SDL Mouse event into internal arrays.

	@param [in,out]	e	The mouse SDL_Event to process.
	*/
	void processMouseEvent(SDL_Event& e);

	/**
	@brief	Process SDL Game Controller event into internal arrays.

	@param [in,out]	e	The Game Controller SDL_Event to process.
	*/
	void processGameControllerEvent(SDL_Event& e);

	/** @brief	Updates the internal variables. */
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