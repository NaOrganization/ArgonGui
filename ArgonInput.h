#pragma once

// ----------------------------------------------------------- //
//  [ARGON] [ArgonInput] Enum class declarations
// ----------------------------------------------------------- //

// [ENUM] [ArgonInputSystem] - Maps to the key codes used in the system.
enum class ArKeyCode
{
	Esc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrintScreen, ScrollLock, PauseBreak,
	Tilde, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0, Minus, Plus, Backspace, Insert, Home, PageUp, NumPadLock, NumPadSlash, NumPadStar, NumPadMinus,
	Tab, Q, W, E, R, T, Y, U, I, O, P, LeftBracket, RightBracket, Backslash, Delete, End, PageDown, NumPad7, NumPad8, NumPad9, NumPadPlus,
	CapsLock, A, S, D, F, G, H, J, K, L, Semicolon, Quote, Enter, NumPad4, NumPad5, NumPad6,
	LeftShift, Z, X, C, V, B, N, M, Comma, Period, Slash, RightShift, UpArrow, NumPad1, NumPad2, NumPad3, NumPadEnter,
	LeftCtrl, LeftWin, LeftAlt, Space, RightAlt, RightWin, Menu, RightCtrl, LeftArrow, DownArrow, RightArrow, NumPad0, NumPadPeriod,
	None
};

// [ENUM] [ArgonInputSystem] - Maps to the mouse buttons used in the system.
enum class ArMouseButton
{
	Left, Middle, Right, X1, X2, None
};

// [ENUM] [ArgonInputSystem] - Mouse wheel direction.
enum class ArMouseWheel
{
	Horizontal, Vertical, None
};

// [ENUM] [ArgonInputSystem] - To describe the source of the mouse input.
enum class ArMouseInputSource
{
	Pen, TouchScreen, Mouse, None
};

// [ENUM] [ArgonInputSystem] - Maps to the gamepad buttons used in the system.
enum class ArGamepadButton
{
	FaceLeft,
	FaceRight,
	FaceUp,
	FaceDown,
	LeftBumper,
	RightBumper,
	Back,
	Start,
	LeftThumb,
	RightThumb,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	None
};

// [ENUM] [ArgonInputSystem] - To describe the type of input device in the event.
enum class ArInputDevice
{
	Keyboard,
	Mouse,
	Gamepad,
	Display,
	None
};

// [ENUM] [ArgonInputSystem] - To describe the state of an input action in a frame.
enum class ArInputAction
{
	Down,	// -> Pressed
	Up,		// -> Released
	None
};

// ---------------------------------------------------------- //
//  !. Event data declarations
// ---------------------------------------------------------- //

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for input events.
class ArMouseEventData
{
public:
	ArMouseInputSource source = ArMouseInputSource::None;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for mouse movement events.
class ArMouseMoveEventData final : public ArMouseEventData
{
public:
	ArVec2 position = ArVec2();
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for mouse button events.
class ArMouseButtonEventData final : public ArMouseEventData
{
public:
	ArMouseButton button = ArMouseButton::None;
	bool pressing = false;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for mouse wheel events.
class ArMouseWheelEventData final : public ArMouseEventData
{
public:
	ArVec2 wheelDelta = ArVec2();
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for keyboard events.
class ArKeyboardEventData final
{
public:
	ArKeyCode keyCode = ArKeyCode::None;
	bool pressing = false;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for gamepad events.
class ArGamepadEventData
{
public:
	int index = 0;
};

// [UNIOVARIANT:TYPEN] [ArgonInputSystem] - Data structure for gamepad button events.
class ArGamepadButtonEventData final : public ArGamepadEventData
{
public:
	ArGamepadButton button = ArGamepadButton::None;
	bool pressing = false;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for gamepad left stick events.
class ArGamepadLeftStickEventData final : public ArGamepadEventData
{
public:
	ArVec2 leftStick = ArVec2();
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for gamepad right stick events.
class ArGamepadRightStickEventData final : public ArGamepadEventData
{
public:
	ArVec2 rightStick = ArVec2();
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for gamepad left trigger events.
class ArGamepadLeftTriggerEventData final : public ArGamepadEventData
{
public:
	float leftTrigger = 0.0f;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for gamepad right trigger events.
class ArGamepadRightTriggerEventData final : public ArGamepadEventData
{
public:
	float rightTrigger = 0.0f;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for noticing the gamepad disconnection.
class ArGamepadConnectionEventData final : public ArGamepadEventData
{
public:
	bool connected = false;
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for display events.
class ArDisplayEventData final
{
public:
	ArVec2 position = ArVec2();
	ArVec2 size = ArVec2();
};

// [VARIANT:TYPE] [ArgonInputSystem] - Data structure for character input events.
class ArInputCharacterEventData final
{
public:
	std::vector<ArChar> characters = {};
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonInput] Input event declarations
// ----------------------------------------------------------- //

// [STRUCT] [ArInputEvent] - Input event structure.
class ArInputEvent final
{
public:
	ArInputDevice deviceType = ArInputDevice::None;
	ArInputEventData eventData = {};
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonInput] Devices states declarations
// ----------------------------------------------------------- //

// [STRUCT] [ArgonInputSystem] - State structure for input buttons.
class ArButtonState final
{
public:
	ArInputAction frameAction = ArInputAction::None;
	bool pressing = false;
	std::vector<ArTimePoint> pressTimes = {};
};

class ArMouseButtonState final
{
public:
	class PressedState final
	{
	public:
		ArInputAction frameAction = ArInputAction::None;
		ArTimePoint pressTime = ArTimePoint();
		ArVec2 pressPosition = ArVec2();

		PressedState(const ArTimePoint& pressTime, const ArVec2& pressPosition, ArInputAction frameAction = ArInputAction::None)
			: pressTime(pressTime), pressPosition(pressPosition), frameAction(frameAction) {
		}
	};

	ArInputAction frameAction = ArInputAction::None;
	bool pressing = false;
	std::vector<PressedState> pressStates = {};
};

// [STRUCT] [ArgonInputSystem] - Base class for input device states.
class ArInputDeviceState
{
public:
	bool enabled = false;

	virtual ~ArInputDeviceState() {}
	virtual void HandleEvent(const ArInputEvent& event) = 0;
	virtual void EndFrame() {};
};

// [STRUCT] [ArgonInputSystem] - State structure for mouse input.
class ArMouseState final : public ArInputDeviceState
{
public:
	ArMouseInputSource inputSource = ArMouseInputSource::None;
	ArVec2 position = ArVec2();
	ArVec2 lastPosition = ArVec2();
	ArVec2 delta = ArVec2();
	ArVec2 wheelDelta = ArVec2();

	std::unordered_map<ArMouseButton, ArMouseButtonState> buttonStates = {};

	void HandleEvent(const ArInputEvent& event) override;
	void EndFrame() override;
};

// [STRUCT] [ArgonInputSystem] - State structure for keyboard input.
class ArKeyboardState final : public ArInputDeviceState
{
public:
	std::unordered_map<ArKeyCode, ArButtonState> keyStates = {};

	void HandleEvent(const ArInputEvent& event) override;
	void EndFrame() override;
};

// [STRUCT] [ArgonInputSystem] - State structure for gamepad input.
class ArGamepadState final : public ArInputDeviceState
{
public:
	ArVec2 leftStick = ArVec2();
	ArVec2 rightStick = ArVec2();
	float leftTrigger = 0.0f;
	float rightTrigger = 0.0f;

	std::unordered_map<ArGamepadButton, ArButtonState> buttonStates = {};

	void HandleEvent(const ArInputEvent& event) override;
	void EndFrame() override;
};

// [STRUCT] [ArgonInputSystem] - State structure for display input.
class ArDisplayState final : public ArInputDeviceState
{
public:
	ArVec2 position = ArVec2();
	ArVec2 size = ArVec2();

	void HandleEvent(const ArInputEvent& event);
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonInput] ArgonInputManager declarations
// ----------------------------------------------------------- //

// [CLASS] [ArgonInputSystem] - The core element, managing all input events and states.
class ArgonInputManager final
{
private:
	std::queue<ArInputEvent> eventQueue = {};
	std::queue<ArInputEvent> eventBuffer = {};
	mutable std::mutex eventMutex;

	ArInputDevice lastInputDevice = ArInputDevice::None;

	ArMouseState mouseState = ArMouseState();
	ArKeyboardState keyboardState = ArKeyboardState();
	std::array<ArGamepadState, 4> gamepadStates = {}; // Assuming a maximum of 4 gamepads
	ArDisplayState displayState = ArDisplayState();

	std::queue<ArChar> inputCharacterQueue = {};
public:
	static constexpr const ArDuration MaxClickInterval = 500ms;
	static constexpr const float MouseClickedDeltaAllowance = 5.f; // Allowance for mouse click movement
	// Gamepad Dead Zone
	float gamepadStickDeadZone = 0.1f;
	float gamepadTriggerDeadZone = 0.05f;
public:
	ArgonInputManager() {}
	ArgonInputManager(const ArgonInputManager&) = delete;
	ArgonInputManager& operator=(const ArgonInputManager&) = delete;
	~ArgonInputManager() {}

	void StartFrame();
	void EndFrame();

	void AddEvent(const ArInputEvent& event);

	ArInputDevice GetLastInputDevice() const { return lastInputDevice; }

	bool IsMouseButtonDown(ArMouseButton button) const;
	bool IsMouseButtonHeld(ArMouseButton button) const;
	bool IsMouseButtonUp(ArMouseButton button) const;
	bool IsMouseButtonClicked(ArMouseButton button, int clickCount = 1) const;

	ArVec2 GetMousePosition() const;
	ArVec2 GetMouseDelta() const;
	ArVec2 GetMouseWheelDelta() const;

	bool IsKeyDown(ArKeyCode keyCode) const;
	bool IsKeyHeld(ArKeyCode keyCode) const;
	bool IsKeyUp(ArKeyCode keyCode) const;
	bool IsKeyCombinationPressed(const std::vector<ArKeyCode>& keys) const;

	bool IsGamepadButtonDown(ArGamepadButton button, int index = 0) const;
	bool IsGamepadButtonHeld(ArGamepadButton button, int index = 0) const;
	bool IsGamepadButtonUp(ArGamepadButton button, int index = 0) const;
	bool IsGamepadConnected(int index = 0) const;
	ArVec2 GetGamepadLeftStick(int index = 0) const;
	ArVec2 GetGamepadRightStick(int index = 0) const;
	float GetGamepadLeftTrigger(int index = 0) const;
	float GetGamepadRightTrigger(int index = 0) const;
	size_t GetGamepadConnectedCount() const;

	ArDisplayState GetDisplayState() const;
private:
	void ParseEvent(const ArInputEvent& event);
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonInput] Interfaces declarations
// ----------------------------------------------------------- //

// [INTERFACE] [ArgonInputSystem] - Interface for platform-specific configurations.
class IArPlatformConfig
{
public:
	virtual ~IArPlatformConfig() {}
};

// [INTERFACE] [ArgonInputSystem] - Interface for platform-specific implementations.
class IArgonPlatform
{
public:
	~IArgonPlatform() {}

	virtual ArString Name() const = 0;

	virtual bool Awake(const IArPlatformConfig& config) = 0;

	virtual void StartFrame(ArgonInputManager& inputManager) = 0;

	virtual bool IsRunning() const = 0;
};