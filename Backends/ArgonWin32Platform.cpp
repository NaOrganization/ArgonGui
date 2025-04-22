#include "ArgonWin32Platform.h"
#include "../ArgonCore.h"
#include "windowsx.h"
#include "xinput.h"

#define DBT_DEVNODES_CHANGED 0x0007

ArKeyCode Win32KeyCodeToArKeyCode(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_ESCAPE: return ArKeyCode::Esc;
	case VK_F1: return ArKeyCode::F1;
	case VK_F2: return ArKeyCode::F2;
	case VK_F3: return ArKeyCode::F3;
	case VK_F4: return ArKeyCode::F4;
	case VK_F5: return ArKeyCode::F5;
	case VK_F6: return ArKeyCode::F6;
	case VK_F7: return ArKeyCode::F7;
	case VK_F8: return ArKeyCode::F8;
	case VK_F9: return ArKeyCode::F9;
	case VK_F10: return ArKeyCode::F10;
	case VK_F11: return ArKeyCode::F11;
	case VK_F12: return ArKeyCode::F12;
	case VK_SNAPSHOT: return ArKeyCode::PrintScreen;
	case VK_SCROLL: return ArKeyCode::ScrollLock;
	case VK_PAUSE: return ArKeyCode::PauseBreak;
	case VK_OEM_3: return ArKeyCode::Tilde;
	case '1': return ArKeyCode::Num1;
	case '2': return ArKeyCode::Num2;
	case '3': return ArKeyCode::Num3;
	case '4': return ArKeyCode::Num4;
	case '5': return ArKeyCode::Num5;
	case '6': return ArKeyCode::Num6;
	case '7': return ArKeyCode::Num7;
	case '8': return ArKeyCode::Num8;
	case '9': return ArKeyCode::Num9;
	case '0': return ArKeyCode::Num0;
	case VK_OEM_MINUS: return ArKeyCode::Minus;
	case VK_OEM_PLUS: return ArKeyCode::Plus;
	case VK_BACK: return ArKeyCode::Backspace;
	case VK_INSERT: return ArKeyCode::Insert;
	case VK_HOME: return ArKeyCode::Home;
	case VK_PRIOR: return ArKeyCode::PageUp;
	case VK_NUMLOCK: return ArKeyCode::NumPadLock;
	case VK_DIVIDE: return ArKeyCode::NumPadSlash;
	case VK_MULTIPLY: return ArKeyCode::NumPadStar;
	case VK_SUBTRACT: return ArKeyCode::NumPadMinus;
	case VK_TAB: return ArKeyCode::Tab;
	case 'Q': return ArKeyCode::Q;
	case 'W': return ArKeyCode::W;
	case 'E': return ArKeyCode::E;
	case 'R': return ArKeyCode::R;
	case 'T': return ArKeyCode::T;
	case 'Y': return ArKeyCode::Y;
	case 'U': return ArKeyCode::U;
	case 'I': return ArKeyCode::I;
	case 'O': return ArKeyCode::O;
	case 'P': return ArKeyCode::P;
	case VK_OEM_4: return ArKeyCode::LeftBracket;
	case VK_OEM_6: return ArKeyCode::RightBracket;
	case VK_OEM_5: return ArKeyCode::Backslash;
	case VK_DELETE: return ArKeyCode::Delete;
	case VK_END: return ArKeyCode::End;
	case VK_NEXT: return ArKeyCode::PageDown;
	case VK_NUMPAD7: return ArKeyCode::NumPad7;
	case VK_NUMPAD8: return ArKeyCode::NumPad8;
	case VK_NUMPAD9: return ArKeyCode::NumPad9;
	case VK_ADD: return ArKeyCode::NumPadPlus;
	case VK_CAPITAL: return ArKeyCode::CapsLock;
	case 'A': return ArKeyCode::A;
	case 'S': return ArKeyCode::S;
	case 'D': return ArKeyCode::D;
	case 'F': return ArKeyCode::F;
	case 'G': return ArKeyCode::G;
	case 'H': return ArKeyCode::H;
	case 'J': return ArKeyCode::J;
	case 'K': return ArKeyCode::K;
	case 'L': return ArKeyCode::L;
	case VK_OEM_1: return ArKeyCode::Semicolon;
	case VK_OEM_7: return ArKeyCode::Quote;
	case VK_RETURN: return ArKeyCode::Enter;
	case VK_NUMPAD4: return ArKeyCode::NumPad4;
	case VK_NUMPAD5: return ArKeyCode::NumPad5;
	case VK_NUMPAD6: return ArKeyCode::NumPad6;
	case VK_SHIFT: return ArKeyCode::LeftShift;
	case 'Z': return ArKeyCode::Z;
	case 'X': return ArKeyCode::X;
	case 'C': return ArKeyCode::C;
	case 'V': return ArKeyCode::V;
	case 'B': return ArKeyCode::B;
	case 'N': return ArKeyCode::N;
	case 'M': return ArKeyCode::M;
	case VK_OEM_COMMA: return ArKeyCode::Comma;
	case VK_OEM_PERIOD: return ArKeyCode::Period;
	case VK_OEM_2: return ArKeyCode::Slash;
	case VK_RSHIFT: return ArKeyCode::RightShift;
	case VK_UP: return ArKeyCode::UpArrow;
	case VK_NUMPAD1: return ArKeyCode::NumPad1;
	case VK_NUMPAD2: return ArKeyCode::NumPad2;
	case VK_NUMPAD3: return ArKeyCode::NumPad3;
	case VK_CONTROL: return ArKeyCode::LeftCtrl;
	case VK_LWIN: return ArKeyCode::LeftWin;
	case VK_MENU: return ArKeyCode::LeftAlt;
	case VK_SPACE: return ArKeyCode::Space;
	case VK_RMENU: return ArKeyCode::RightAlt;
	case VK_RWIN: return ArKeyCode::RightWin;
	case VK_APPS: return ArKeyCode::Menu;
	case VK_RCONTROL: return ArKeyCode::RightCtrl;
	case VK_LEFT: return ArKeyCode::LeftArrow;
	case VK_DOWN: return ArKeyCode::DownArrow;
	case VK_RIGHT: return ArKeyCode::RightArrow;
	case VK_NUMPAD0: return ArKeyCode::NumPad0;
	case VK_DECIMAL: return ArKeyCode::NumPadPeriod;
	default: return ArKeyCode::None;
	}
}

ArGamepadButton BitToArGameButton(int bit)
{
	switch (bit)
	{
	case XINPUT_GAMEPAD_DPAD_UP: return ArGamepadButton::DPadUp;
	case XINPUT_GAMEPAD_DPAD_DOWN: return ArGamepadButton::DPadDown;
	case XINPUT_GAMEPAD_DPAD_LEFT: return ArGamepadButton::DPadLeft;
	case XINPUT_GAMEPAD_DPAD_RIGHT: return ArGamepadButton::DPadRight;
	case XINPUT_GAMEPAD_START: return ArGamepadButton::Start;
	case XINPUT_GAMEPAD_BACK: return ArGamepadButton::Back;
	case XINPUT_GAMEPAD_LEFT_THUMB: return ArGamepadButton::LeftThumb;
	case XINPUT_GAMEPAD_RIGHT_THUMB: return ArGamepadButton::RightThumb;
	case XINPUT_GAMEPAD_LEFT_SHOULDER: return ArGamepadButton::LeftBumper;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER: return ArGamepadButton::RightBumper;
	case XINPUT_GAMEPAD_A: return ArGamepadButton::FaceDown;
	case XINPUT_GAMEPAD_B: return ArGamepadButton::FaceRight;
	case XINPUT_GAMEPAD_X: return ArGamepadButton::FaceLeft;
	case XINPUT_GAMEPAD_Y: return ArGamepadButton::FaceUp;
	default: return ArGamepadButton::None;
	}
}

ArMouseInputSource Win32MouseInputSourceToArMouseInputSource()
{
	LPARAM extra_info = ::GetMessageExtraInfo();
	if ((extra_info & 0xFFFFFF80) == 0xFF515700)
		return ArMouseInputSource::Pen;
	if ((extra_info & 0xFFFFFF80) == 0xFF515780)
		return ArMouseInputSource::TouchScreen;
	return ArMouseInputSource::Mouse;
}

bool IsVkDown(int vk)
{
	return (::GetKeyState(vk) & 0x8000) != 0;
}

DWORD WINAPI XInputGetCapabilitiesWrapper(void* address, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
{
	return ((DWORD(WINAPI*)(DWORD, DWORD, XINPUT_CAPABILITIES*))(address))(dwUserIndex, dwFlags, pCapabilities);
}

DWORD WINAPI XInputGetStateWrapper(void* address, DWORD dwUserIndex, XINPUT_STATE* pState)
{
	return ((DWORD(WINAPI*)(DWORD, XINPUT_STATE*))(address))(dwUserIndex, pState);
}

DWORD WINAPI XInputSetStateWrapper(void* address, DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
	return ((DWORD(WINAPI*)(DWORD, XINPUT_VIBRATION*))(address))(dwUserIndex, pVibration);
}

bool ArgonWin32Platform::Awake(const IArPlatformConfig& config)
{
	const ArWin32PlatformConfig& win32Config = static_cast<const ArWin32PlatformConfig&>(config);
	windowHwnd = win32Config.windowHandle;
	std::vector<std::string> xinput_dll_names =
	{
		"xinput1_4.dll",   // Windows 8+
		"xinput1_3.dll",   // DirectX SDK
		"xinput9_1_0.dll", // Windows Vista, Windows 7
		"xinput1_2.dll",   // DirectX SDK
		"xinput1_1.dll"    // DirectX SDK
	};
	for (const std::string& dll_name : xinput_dll_names)
	{
		if (HMODULE dll = ::LoadLibraryA(dll_name.c_str()))
		{
			xinputDLL = dll;
			if (xinputGetCapabilities = ::GetProcAddress(dll, "XInputGetCapabilities"); !xinputGetCapabilities)
				throw std::runtime_error("Failed to load XInputGetCapabilities from " + dll_name);
			if (xinputGetState = ::GetProcAddress(dll, "XInputGetState"); !xinputGetState)
				throw std::runtime_error("Failed to load XInputGetState from " + dll_name);
			if (xinputSetState = ::GetProcAddress(dll, "XInputSetState"); !xinputSetState)
				throw std::runtime_error("Failed to load XInputSetState from " + dll_name);
			break;
		}
	}
	return true;
}

void ArgonWin32Platform::OnDestroy()
{
	if (xinputDLL)
		FreeModule(xinputDLL);
}

void ArgonWin32Platform::StartFrame(ArgonInputManager& inputManager)
{
	GamepadUpdate(inputManager);
	WindowUpdate(inputManager);
	MouseUpdate(inputManager);
}

void ArgonWin32Platform::GamepadUpdate(ArgonInputManager& inputManager)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (!xinputConnected[i])
			return;
		XINPUT_STATE state = {};
		if (!xinputGetState || XInputGetStateWrapper(xinputGetState, i, &state) != ERROR_SUCCESS)
			continue;
		if (state.dwPacketNumber == xinputStates[i].dwPacketNumber)
			continue;
		XINPUT_GAMEPAD& gamepad = state.Gamepad;
		const XINPUT_GAMEPAD& lastFrameGamepad = xinputStates[i].Gamepad;

		if (gamepad.wButtons != lastFrameGamepad.wButtons)
		{
			//#define XINPUT_GAMEPAD_DPAD_UP          0x0001 -> 0000000000000001
			//#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002 -> 0000000000000010
			//#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004 -> 0000000000000100
			//#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008 -> 0000000000001000
			//#define XINPUT_GAMEPAD_START            0x0010 -> 0000000000010000
			//#define XINPUT_GAMEPAD_BACK             0x0020 -> 0000000000100000
			//#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040 -> 0000000001000000
			//#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080 -> 0000000010000000
			//#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100 -> 0000000100000000
			//#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200 -> 0000001000000000
			//#define XINPUT_GAMEPAD_A                0x1000 -> 0001000000000000
			//#define XINPUT_GAMEPAD_B                0x2000 -> 0010000000000000
			//#define XINPUT_GAMEPAD_X                0x4000 -> 0100000000000000
			//#define XINPUT_GAMEPAD_Y                0x8000 -> 1000000000000000
			for (int j = 0; j < 16; ++j)
			{
				if ((gamepad.wButtons & (1 << j)) != (lastFrameGamepad.wButtons & (1 << j)))
				{
					ArGamepadButton button = BitToArGameButton(1 << j);
					bool pressing = (gamepad.wButtons & (1 << j)) != 0;
					ArGamepadButtonEventData eventData = {};
					ArInputEvent event = {};
					eventData.button = button;
					eventData.pressing = pressing;
					event.deviceType = ArInputDevice::Gamepad;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
			}
		}
		else if (gamepad.sThumbLX != lastFrameGamepad.sThumbLX || gamepad.sThumbLY != lastFrameGamepad.sThumbLY)
		{
			ArVec2 leftStick = ArVec2((float)gamepad.sThumbLX / 32767.0f, (float)gamepad.sThumbLY / 32767.0f);
			float leftStickMagnitude = sqrtf(leftStick.x * leftStick.x + leftStick.y * leftStick.y);
			if (leftStickMagnitude > inputManager.gamepadStickDeadZone)
			{
				leftStick.x /= leftStickMagnitude;
				leftStick.y /= leftStickMagnitude;
			}
			else
			{
				leftStick = ArVec2(0.0f, 0.0f);
			}
			ArGamepadLeftStickEventData eventData = {};
			ArInputEvent event = {};
			eventData.leftStick = leftStick;
			eventData.index = i;
			event.deviceType = ArInputDevice::Gamepad;
			event.eventData = eventData;
			inputManager.AddEvent(event);
		}
		else if (gamepad.sThumbRX != lastFrameGamepad.sThumbRX || gamepad.sThumbRY != lastFrameGamepad.sThumbRY)
		{
			ArVec2 rightStick = ArVec2((float)gamepad.sThumbRX / 32767.0f, (float)gamepad.sThumbRY / 32767.0f);
			float rightStickMagnitude = sqrtf(rightStick.x * rightStick.x + rightStick.y * rightStick.y);
			if (rightStickMagnitude > inputManager.gamepadStickDeadZone)
			{
				rightStick.x /= rightStickMagnitude;
				rightStick.y /= rightStickMagnitude;
			}
			else
			{
				rightStick = ArVec2(0.0f, 0.0f);
			}
			ArGamepadRightStickEventData eventData = {};
			ArInputEvent event = {};
			eventData.rightStick = rightStick;
			eventData.index = i;
			event.deviceType = ArInputDevice::Gamepad;
			event.eventData = eventData;
			inputManager.AddEvent(event);
		}
		else if (gamepad.bLeftTrigger != lastFrameGamepad.bLeftTrigger)
		{
			float leftTrigger = (float)gamepad.bLeftTrigger / 255.0f;
			if (leftTrigger < inputManager.gamepadTriggerDeadZone)
				leftTrigger = 0.0f;
			ArGamepadLeftTriggerEventData eventData = {};
			ArInputEvent event = {};
			eventData.leftTrigger = leftTrigger;
			eventData.index = i;
			event.deviceType = ArInputDevice::Gamepad;
			event.eventData = eventData;
			inputManager.AddEvent(event);
		}
		else if (gamepad.bRightTrigger != lastFrameGamepad.bRightTrigger)
		{
			float rightTrigger = (float)gamepad.bRightTrigger / 255.0f;
			if (rightTrigger < inputManager.gamepadTriggerDeadZone)
				rightTrigger = 0.0f;
			ArGamepadRightTriggerEventData eventData = {};
			ArInputEvent event = {};
			eventData.rightTrigger = rightTrigger;
			eventData.index = i;
			event.deviceType = ArInputDevice::Gamepad;
			event.eventData = eventData;
			inputManager.AddEvent(event);
		}
		xinputStates[i] = state;
	}
}

void ArgonWin32Platform::WindowUpdate(ArgonInputManager& inputManager)
{
	RECT rect = { 0, 0, 0, 0 };
	::GetClientRect(windowHwnd, &rect);
	ArDisplayEventData eventData = {};
	ArInputEvent event = {};
	eventData.position = ArVec2((float)rect.left, (float)rect.top);
	eventData.size = ArVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
	event.deviceType = ArInputDevice::Display;
	event.eventData = eventData;
	inputManager.AddEvent(event);
}

void ArgonWin32Platform::MouseUpdate(ArgonInputManager& inputManager)
{
	HWND focused_window = ::GetForegroundWindow();
	const bool is_app_focused = (focused_window == mouseHwnd);
	if (!is_app_focused)
		return;
	if (mouseTrackedArea != 0)
		return;
	POINT pos = {};
	if (!::GetCursorPos(&pos) || !::ScreenToClient(windowHwnd, &pos))
		return;
	ArMouseMoveEventData eventData = {};
	ArInputEvent event = {};
	eventData.source = Win32MouseInputSourceToArMouseInputSource();
	eventData.position = ArVec2((float)pos.x, (float)pos.y);
	event.deviceType = ArInputDevice::Mouse;
	event.eventData = eventData;
	inputManager.AddEvent(event);
}

void ArgonWin32Platform::TryGamepads()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_CAPABILITIES caps = {};
		if (!xinputGetCapabilities || XInputGetCapabilitiesWrapper(xinputGetCapabilities, i, XINPUT_FLAG_GAMEPAD, &caps) != ERROR_SUCCESS)
			continue;
		xinputConnected[i] = true;
	}
}

LRESULT WINAPI ArWin32WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!ArgonContext::GetInstance().IsRunning())
		return 0;
	ArgonInputManager& inputManager = ArgonContext::GetInstance().inputManager;
	ArgonWin32Platform* platform = (ArgonWin32Platform*)ArgonContext::GetInstance().platform;
	switch (msg)
	{
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	{
		const int area = (msg == WM_MOUSEMOVE) ? 1 : 2;
		platform->mouseHwnd = hWnd;
		if (platform->mouseTrackedArea != area)
		{
			TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hWnd, 0 };
			TRACKMOUSEEVENT tme_track = { sizeof(tme_track), (DWORD)((area == 2) ? (TME_LEAVE | TME_NONCLIENT) : TME_LEAVE), hWnd, 0 };
			if (platform->mouseTrackedArea != 0)
				::TrackMouseEvent(&tme_cancel);
			::TrackMouseEvent(&tme_track);
			platform->mouseTrackedArea = area;
		}
		POINT mouse_pos = { (LONG)GET_X_LPARAM(lParam), (LONG)GET_Y_LPARAM(lParam) };
		if (msg == WM_NCMOUSEMOVE && ::ScreenToClient(hWnd, &mouse_pos) == FALSE) // WM_NCMOUSEMOVE are provided in absolute coordinates.
			break;
		ArMouseMoveEventData eventData = {};
		ArInputEvent event = {};
		eventData.source = Win32MouseInputSourceToArMouseInputSource();
		eventData.position = ArVec2((float)mouse_pos.x, (float)mouse_pos.y);
		event.deviceType = ArInputDevice::Mouse;
		event.eventData = eventData;
		inputManager.AddEvent(event);
		return 0;
	}
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
	{
		const int area = (msg == WM_MOUSELEAVE) ? 1 : 2;
		if (platform->mouseTrackedArea != area)
			return 0;
		if (platform->mouseHwnd == hWnd)
			platform->mouseHwnd = nullptr;
		platform->mouseTrackedArea = 0;

		ArMouseMoveEventData eventData = {};
		ArInputEvent event = {};
		eventData.source = Win32MouseInputSourceToArMouseInputSource();
		eventData.position = ArVec2(-FLT_MAX, -FLT_MAX);
		event.deviceType = ArInputDevice::Mouse;
		event.eventData = eventData;
		inputManager.AddEvent(event);
		platform->mouseTrackedArea = 0;
		return 0;
	}
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		ArMouseButton button = ArMouseButton::None;
		switch (msg)
		{
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK: button = ArMouseButton::Left; break;
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK: button = ArMouseButton::Right; break;
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK: button = ArMouseButton::Middle; break;
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK: button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? ArMouseButton::X1 : ArMouseButton::X2; break;
		}
		ArMouseButtonEventData eventData = {};
		ArInputEvent event = {};
		eventData.button = button;
		eventData.pressing = true;
		event.deviceType = ArInputDevice::Mouse;
		event.eventData = eventData;
		inputManager.AddEvent(event);
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		ArMouseButton button = ArMouseButton::None;
		switch (msg)
		{
		case WM_LBUTTONUP: button = ArMouseButton::Left; break;
		case WM_RBUTTONUP: button = ArMouseButton::Right; break;
		case WM_MBUTTONUP: button = ArMouseButton::Middle; break;
		case WM_XBUTTONUP: button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? ArMouseButton::X1 : ArMouseButton::X2; break;
		}
		ArMouseButtonEventData eventData = {};
		ArInputEvent event = {};
		eventData.button = button;
		eventData.pressing = false;
		event.deviceType = ArInputDevice::Mouse;
		event.eventData = eventData;
		inputManager.AddEvent(event);
		return 0;
	}
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	{
		ArMouseWheelEventData eventData = {};
		ArInputEvent event = {};
		eventData.source = Win32MouseInputSourceToArMouseInputSource();
		if (msg == WM_MOUSEWHEEL)
			eventData.wheelDelta = ArVec2(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		else
			eventData.wheelDelta = ArVec2((float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA, 0.0f);
		event.deviceType = ArInputDevice::Mouse;
		event.eventData = eventData;
		inputManager.AddEvent(event);
		return 0;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	{
		const bool is_key_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
		if (wParam < 256)
		{
			int vk = (int)wParam;
			if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
				vk = VK_RETURN; // Numpad Enter

			// Submit key event
			const ArKeyCode key = Win32KeyCodeToArKeyCode(wParam);
			const int scancode = (int)LOBYTE(HIWORD(lParam));
			if (key != ArKeyCode::None)
			{
				ArKeyboardEventData eventData = {};
				ArInputEvent event = {};
				eventData.keyCode = key;
				eventData.pressing = is_key_down;
				event.deviceType = ArInputDevice::Keyboard;
				event.eventData = eventData;
				inputManager.AddEvent(event);
			}

			// Submit individual left/right modifier events
			if (vk == VK_SHIFT)
			{
				// Important: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in ImGui_ImplWin32_ProcessKeyEventsWorkarounds()
				if (IsVkDown(VK_LSHIFT) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::LeftShift;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
				if (IsVkDown(VK_RSHIFT) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::RightShift;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
			}
			else if (vk == VK_CONTROL)
			{
				if (IsVkDown(VK_LCONTROL) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::LeftCtrl;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
				if (IsVkDown(VK_RCONTROL) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::RightCtrl;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
			}
			else if (vk == VK_MENU)
			{
				if (IsVkDown(VK_LMENU) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::LeftAlt;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
				if (IsVkDown(VK_RMENU) == is_key_down)
				{
					ArKeyboardEventData eventData = {};
					ArInputEvent event = {};
					eventData.keyCode = ArKeyCode::RightAlt;
					eventData.pressing = is_key_down;
					event.deviceType = ArInputDevice::Keyboard;
					event.eventData = eventData;
					inputManager.AddEvent(event);
				}
			}
		}
		return 0;
	}
	case WM_CHAR:
	{

	}
	case WM_SETCURSOR:
	{
		if (LOWORD(lParam) == HTCLIENT)
		{
			::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
			return TRUE;
		}
		return 0;
	}
	case WM_DEVICECHANGE:
		if ((UINT)wParam == DBT_DEVNODES_CHANGED)
			platform->TryGamepads();
	default:
		return 0;
	}
	return 0;
}