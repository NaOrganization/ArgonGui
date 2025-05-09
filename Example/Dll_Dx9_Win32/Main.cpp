#include "Main.h"

class Hooker
{
private:
	void* detour = nullptr;
	void** original = nullptr;

public:
	bool hooked = false;

	Hooker(void* detour, void** original) : detour(detour), original(original) {}

	bool Hook(void* target)
	{
		*original = target;
		if (DetourTransactionBegin() != NO_ERROR)
		{
			return false;
		}
		if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
		{
			return false;
		}
		if (DetourAttach(original, detour) != NO_ERROR)
		{
			return false;
		}
		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}
		return hooked = true;
	}

	bool UnHook()
	{
		if (DetourTransactionBegin() != NO_ERROR)
		{
			return false;
		}
		if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
		{
			return false;
		}
		if (DetourDetach(original, detour) != NO_ERROR)
		{
			return false;
		}
		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}
		return !(hooked = false);
	}
};

#define RAW_HOOK(_returnType, _funcName, ...) \
	_returnType h##_funcName(__VA_ARGS__);             \
	using _funcName##Fn = decltype(&h##_funcName);     \
	inline _funcName##Fn o##_funcName = nullptr;    \
	inline Hooker hi##_funcName = Hooker(h##_funcName, reinterpret_cast<void **>(&o##_funcName)); \

#define HOOK(_returnType, _funcName, ...) \
		RAW_HOOK(_returnType, _funcName, __VA_ARGS__) \
		inline _returnType h##_funcName(__VA_ARGS__)

namespace d3d9
{
	IDirect3DDevice9* Device = NULL;
	D3DDEVICE_CREATION_PARAMETERS CreationParams = {};
	ArgonDx9Renderer* ArgonRenderer = nullptr;

	HOOK(LRESULT WINAPI, WndProc, const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (unloaded)
			return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);

		if (uMsg == WM_KEYDOWN && wParam == VK_HOME)
		{
			unloaded = true;
			return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
		}

		if (ArWin32WndProc(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	HOOK(HRESULT __stdcall, Present, IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
	{
		if (unloaded)
			return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		if (Device == nullptr)
		{
			Device = pDevice;

			if (FAILED(Device->GetCreationParameters(&CreationParams)))
			{
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}
			
			if (!hiWndProc.Hook((void*)GetWindowLongPtr(CreationParams.hFocusWindow, GWLP_WNDPROC)))
			{
				unloaded = true;
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}

			if (!ArGui::SetPlatform(new ArgonWin32Platform(), ArWin32PlatformConfig(CreationParams.hFocusWindow)))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}
			ArgonRenderer = new ArgonDx9Renderer();
			if (!ArGui::SetRenderer(ArgonRenderer, ArDx9RendererConfig(Device)))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}

			if (!ArGui::SetGlyphParser(new ArgonFreeTypeGlyphParser(), {}))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}

			// english
			ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\CascadiaCode.ttf");
			// chinese
			ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\simhei.ttf");
			// emoji
			ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\seguiemj.ttf");

			if (!ArGui::Initialize())
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}
		}
		ArGui::FrameUpdate();
		if (Device->BeginScene() == D3D_OK)
		{
			ArGui::Present();
			Device->EndScene();
		}
		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	HOOK(HRESULT __stdcall, Reset, IDirect3DDevice9* pDevice, CONST D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		if (unloaded || Device == nullptr)
			return oReset(pDevice, pPresentationParameters);
		ArgonRenderer->ReleaseRendererObjects();
		HRESULT hr = oReset(pDevice, pPresentationParameters);
		ArgonRenderer->CreateRendererObjects();
		return hr;
	}
}

inline bool ScanD3D9Function(void**& vTable)
{
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		DefWindowProc,
		0,
		0,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"DirectX 9",
		NULL,
	};

	RegisterClassEx(&windowClass);

	HWND window = CreateWindow(windowClass.lpszClassName, L"DirectX 9 Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

	IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)
	{
		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		return false;
	}

	D3DPRESENT_PARAMETERS params = {};
	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D16;
	params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	IDirect3DDevice9* device = nullptr;
	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device)))
	{
		d3d->Release();
		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		return false;
	}
	vTable = *(void***)device;

	d3d->Release();

	device->Release();

	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	return true;
}

bool DllInitialize()
{
	void** dxgiFunctionVTable = nullptr;
	if (!ScanD3D9Function(dxgiFunctionVTable))
	{
		return false;
	}

	if (!d3d9::hiPresent.Hook((void*)dxgiFunctionVTable[17])
		|| !d3d9::hiReset.Hook((void*)dxgiFunctionVTable[16]))
	{
		return false;
	}
	return true;
}

void Main()
{
	unloaded = !DllInitialize();

	while (!unloaded)
	{
		std::this_thread::sleep_for(1s);
	}

	d3d9::hiPresent.UnHook();
	d3d9::hiReset.UnHook();
	d3d9::hiWndProc.UnHook();

	ArGui::Shutdown();
	FreeLibraryAndExitThread(selfModule, 0);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		selfModule = module;

		auto currentProcess = GetCurrentProcess();
		auto priorityClass = GetPriorityClass(currentProcess);
		if (priorityClass != HIGH_PRIORITY_CLASS && priorityClass != REALTIME_PRIORITY_CLASS)
			SetPriorityClass(currentProcess, HIGH_PRIORITY_CLASS);
		DisableThreadLibraryCalls(module);
		CreateThread(LPSECURITY_ATTRIBUTES{}, SIZE_T(NULL), (LPTHREAD_START_ROUTINE)Main, LPVOID(NULL), DWORD(NULL), LPDWORD(NULL));
	}
	return TRUE;
}
