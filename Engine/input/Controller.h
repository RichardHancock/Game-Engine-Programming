#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <SDL.h>
#include <memory>

#include "../misc/Vec2.h"
#include "../misc/CustomDestructors.h"

/** @brief	A game controller wrapper around SDL's game controller functionality. Also supports haptics. */
class Controller
{
public:
	
	/** @brief	Values that represent 1 dimensional axis. */
	enum Axis1D {
		LeftTrigger,
		RightTrigger
	};

	/** @brief	Values that represent 2 dimensional axis. */
	enum Axis2D {
		LeftStick,
		RightStick
	};
	
	/** @brief	Values that represent the buttons commonly found on a controller. */
	enum Button
	{
		A = SDL_CONTROLLER_BUTTON_A,
		B = SDL_CONTROLLER_BUTTON_B,
		X = SDL_CONTROLLER_BUTTON_X,
		Y = SDL_CONTROLLER_BUTTON_Y,
		BACK = SDL_CONTROLLER_BUTTON_BACK,
		GUIDE = SDL_CONTROLLER_BUTTON_GUIDE,
		START = SDL_CONTROLLER_BUTTON_START,
		LEFTSTICK = SDL_CONTROLLER_BUTTON_LEFTSTICK,
		RIGHTSTICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		RIGHTSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
		DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT
	};

	//Explanation here: http://www.drdobbs.com/windows/user-defined-hash-functions-for-unordere/231600210?pgno=3
	// Basically non VS compilers require a hash function for all User Defined types.
	struct hash_Button {
		size_t operator()(const Button button) const
		{
			return std::hash<int>()(button);
		}
	};

	/**
	 @brief	Constructor.
	
	 @param	joyID	Identifier for the joy.
	 */
	Controller(int joyID);

	/** @brief	Destructor. */
	~Controller();

	/**
	 @brief	Query if this Controller is valid.
	
	 @return	true if valid, false if not.
	 */
	bool isValid();

	/**
	 @brief	Gets the name of the controller hardware if available.
	
	 @return	The name.
	 */
	std::string getName();

	/**
	 @brief	Gets joystick identifier.
	
	 @return	The joystick identifier.
	 */
	int getJoystickID();

	/**
	 @brief	Gets joystick instance identifier.
	
	 @return	The joystick instance identifier.
	 */
	SDL_JoystickID getJoystickInstanceID();

	/**
	 @brief	Was button pressed.
	
	 @param	button	The button.
	
	 @return	true if pressed.
	 */
	bool wasButtonPressed(Button button);

	/**
	@brief	Is button held.

	@param	button	The button.

	@return	true if held.
	*/
	bool isButtonHeld(Button button);

	/**
	@brief	Was button released.

	@param	button	The button.

	@return	true if released.
	*/
	bool wasButtonReleased(Button button);

	/**
	 @brief	Gets specified 1D axis value.
	
	 @param	axis	The axis.
	
	 @return	The axis value.
	 */
	float getAxis1D(Axis1D axis);

	/**
	@brief	Gets specified 2D axis value.

	@param	axis	The axis.

	@return	The axis value.
	*/
	Vec2 getAxis2D(Axis2D axis);

	/**
	 @brief	Play rumble effect.
	
	 @param	strength	The strength (0-1).
	 @param	lengthMS	The length in milliseconds.
	 */
	void rumblePlay(float strength, uint32_t lengthMS);

	/** @brief	Stop rumble effect. */
	void rumbleStop();

	/**
	 @brief	Process the controller events described by e.
	
	 @param [in,out]	e	The SDL_Event to process.
	 */
	void processEvents(SDL_Event& e);

	/** @brief	Updates this Controller. */
	void update();
private:

	/** @brief	The axis minimum possible value. */
	const Sint16 AXIS_MIN = -32768;
	/** @brief	The axis maximum possible value. */
	const Sint16 AXIS_MAX = 32767;

	/** @brief	Values that represent button states. */
	enum ButtonState {
		Pressed,
		Held,
		Released,
		None,
		Unknown
	};

	/**
	 @brief	Updates the axis described by e.
	
	 @param [in,out]	e	The SDL_Event to process.
	 */
	void updateAxis(SDL_Event& e);

	/**
	 @brief	Updates the buttons described by e.
	
	 @param [in,out]	e	The SDL_Event to process.
	 */
	void updateButtons(SDL_Event& e);

	/** @brief	The game controller internals. */
	std::unique_ptr<SDL_GameController, CustomDestructors::SDL_Deleter> gameController;

	/** @brief	Identifier for the joystick. */
	int joystickID;

	/** @brief	Identifier for the joystick instance. */
	SDL_JoystickID joystickInstanceID;


	/** @brief	The left stick's last reported value. */
	Vec2 leftStick;

	/** @brief	The right stick's last reported value. */
	Vec2 rightStick;

	/** @brief	The left trigger's last reported value. */
	float leftTrigger;

	/** @brief	The right trigger's last reported value. */
	float rightTrigger;

	/** @brief	The buttons and their current states. */
	std::unordered_map<Button, ButtonState, hash_Button> buttons;

	//Haptics

	/**
	 @brief	Initializes the haptics subsystem for this controller.
	
	 @param [in,out]	joystick	The SDL joystick (Passed as pointer as it should never be memory managed as it is an internal pointer to SDL).
	 */
	void initializeHaptics(SDL_Joystick* joystick);

	/** @brief	The haptic subsystem. */
	std::unique_ptr<SDL_Haptic, CustomDestructors::SDL_Deleter> haptic;

	/** @brief	true if rumble supported. */
	bool rumbleSupported;
};