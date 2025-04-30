#pragma once
#include "../ArgonGui.h"
#include <wtypes.h>
#include <xinput.h>

class ArWin32PlatformConfig final : public IArPlatformConfig
{
public:
	HWND windowHandle = nullptr;

	ArWin32PlatformConfig(HWND windowHandle) : windowHandle(windowHandle) {}
};

// [ArgonInputSystem] ArgonPlatform in Win32
class ArgonWin32Platform final : public IArgonPlatform
{
public:
	HWND windowHwnd = nullptr;
	HWND mouseHwnd = nullptr;
	int mouseTrackedArea = 0;
	int mouseButtonsDown = 0;
	HMODULE xinputDLL = nullptr;
	void* xinputGetCapabilities = nullptr;
	void* xinputGetState = nullptr;
	void* xinputSetState = nullptr;
	std::array<XINPUT_STATE, 4> xinputStates = {};
	std::array<bool, 4> xinputConnected = {};
public:
	ArgonWin32Platform() {}

	ArString Name() const override { return "Win32"; }
	bool Awake(const IArPlatformConfig& config) override;
	void OnDestroy() override;
	void StartFrame(ArgonInputManager& inputManager) override;
	bool IsRunning() const override { return xinputGetCapabilities && xinputGetState && xinputSetState && windowHwnd; }
private:
	void GamepadUpdate(ArgonInputManager& inputManager);
	void WindowUpdate(ArgonInputManager& inputManager);
	void MouseUpdate(ArgonInputManager& inputManager);
	void TryGamepads();

	friend LRESULT WINAPI ArWin32WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

// [ArgonInputSystem] Win32 Message handler
LRESULT WINAPI ArWin32WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);