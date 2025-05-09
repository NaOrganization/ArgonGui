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

namespace dxgi
{
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* MainRenderTargetView = nullptr;

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

	HOOK(HRESULT, Present, IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (unloaded)
			return oPresent(pSwapChain, SyncInterval, Flags);
		if (SwapChain == nullptr)
		{
			SwapChain = pSwapChain;

			if (FAILED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))))
			{
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			Device->GetImmediateContext(&DeviceContext);

			DXGI_SWAP_CHAIN_DESC sd = {};
			if (FAILED(SwapChain->GetDesc(&sd)))
			{
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (!hiWndProc.Hook((void*)GetWindowLongPtr(sd.OutputWindow, GWLP_WNDPROC)))
			{
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			ID3D11Texture2D* backBuffer = NULL;
			if (FAILED((SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)))))
			{
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (FAILED(Device->CreateRenderTargetView(backBuffer, nullptr, &MainRenderTargetView)))
			{
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}
			backBuffer->Release();

			if (!ArGui::SetPlatform(new ArgonWin32Platform(), ArWin32PlatformConfig(sd.OutputWindow)))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (!ArGui::SetRenderer(new ArgonDx11Renderer(), ArDx11RendererConfig(SwapChain, Device, DeviceContext)))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (!ArGui::SetGlyphParser(new ArgonFreeTypeGlyphParser(), {}))
			{
				ArGui::Shutdown();
				unloaded = true;
				return oPresent(pSwapChain, SyncInterval, Flags);
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
				return oPresent(pSwapChain, SyncInterval, Flags);
			}
		}
		ArGui::FrameUpdate();
		DeviceContext->OMSetRenderTargets(1, &MainRenderTargetView, nullptr);
		ArGui::Present();
		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	HOOK(HRESULT, ResizeBuffers, IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		if (unloaded)
			return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		if (MainRenderTargetView)
		{
			DeviceContext->OMSetRenderTargets(0, 0, 0);
			MainRenderTargetView->Release();
			MainRenderTargetView = nullptr;
		}
		HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
		ID3D11Texture2D* backBuffer = nullptr;
		if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
		{
			return hr;
		}
		Device->CreateRenderTargetView(backBuffer, nullptr, &MainRenderTargetView);
		backBuffer->Release();
		DeviceContext->OMSetRenderTargets(1, &MainRenderTargetView, nullptr);
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)Width;
		vp.Height = (FLOAT)Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		DeviceContext->RSSetViewports(1, &vp);
		return hr;
	}
}

bool FindIDXGISwapChainVTable(void**& vTable)
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
		L"DirectX 11",
		NULL,
	};

	RegisterClassEx(&windowClass);

	HWND window = CreateWindow(windowClass.lpszClassName, L"DirectX 11 Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_MODE_DESC bufferDesc =
	{
		100,
		100,
		{60, 1},
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		DXGI_MODE_SCALING_UNSPECIFIED,
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc =
	{
		bufferDesc,
		{1, 0},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		1,
		window,
		1,
		DXGI_SWAP_EFFECT_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
	};

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context);

	if (FAILED(result))
	{
		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		return false;
	}

	vTable = *(void***)swapChain;

	if (swapChain)
	{
		swapChain->Release();
		swapChain = NULL;
	}

	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (context)
	{
		context->Release();
		context = NULL;
	}

	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	return true;
}

bool DllInitialize()
{
	void** IDXGISwapChainVTable = nullptr;
	if (!FindIDXGISwapChainVTable(IDXGISwapChainVTable))
	{
		return false;
	}

	if (!dxgi::hiPresent.Hook((void*)IDXGISwapChainVTable[8])
		|| !dxgi::hiResizeBuffers.Hook((void*)IDXGISwapChainVTable[13]))
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

	dxgi::hiPresent.UnHook();
	dxgi::hiResizeBuffers.UnHook();
	dxgi::hiWndProc.UnHook();

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
