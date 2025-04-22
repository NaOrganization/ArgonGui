#include "Main.h"

// Main code
int main(int, char**)
{
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Example", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ArVec4 clear_color = { 0.45f, 0.55f, 0.60f, 1.00f };

	if (!ArGui::SetPlatform(new ArgonWin32Platform(), ArWin32PlatformConfig(hwnd)))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	if (!ArGui::SetRenderer(new ArgonDx11Renderer(), ArDx11RendererConfig(g_pSwapChain, g_pd3dDevice, g_pd3dDeviceContext)))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	if (!ArGui::SetGlyphParser(new ArgonFreeTypeGlyphParser()))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	// english
	ArGui::GetRenderManager().AddFontFromFile("C:\\Windows\\Fonts\\CascadiaCode.ttf");
	// chinese
	ArGui::GetRenderManager().AddFontFromFile("C:\\Windows\\Fonts\\simhei.ttf");
	// emoji
	ArGui::GetRenderManager().AddFontFromFile("C:\\Windows\\Fonts\\seguiemj.ttf");

	if (!ArGui::Initialize())
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	// Main loop
	bool done = false;
	float maxFramePerSecond = 0.f;
	float averageFramePerSecond = 0.f;
	float maxAverageFramePerSecond = 0.f;
	while (!done)
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;
		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}

		//ArGui::FrameUpdate();

		if (ArGui::StartFrame())
		{
			if (ArRenderList* a = ArGui::GetGraphicManager().GetBackgroundRenderList())
			{
				a->AddImage(a->GetCurrentTexture(), ArRect(ArVec2(10.f, 10.f), ArGui::GetContext().inputManager.GetDisplayState().size - ArVec2(10.f, 10.f)), ArRect(0.f, 0.f, 0.4f, 0.4f), AR_COL32_WHITE);

				float fps = 1 / ArGui::GetContext().GetDeltaTime();
				maxFramePerSecond = ArMax(maxFramePerSecond, fps);
				averageFramePerSecond = (averageFramePerSecond + fps) / 2.f;
				maxAverageFramePerSecond = ArMax(maxAverageFramePerSecond, averageFramePerSecond);
				ArString fpsText = fmt::format("FPS: {}", fps);
				ArString maxFPSText = fmt::format("max FPS: {}", maxFramePerSecond);
				ArString averageFPSText = fmt::format("average FPS: {}", averageFramePerSecond);
				ArString maxAverageFPSText = fmt::format("max average FPS: {}", maxAverageFramePerSecond);
				a->AddText(fpsText, 16u, ArVec2(100.f, 100.f), AR_COL32_WHITE);
				a->AddText(maxFPSText, 16u, ArVec2(100.f, 116.f), AR_COL32_WHITE);
				a->AddText(averageFPSText, 16u, ArVec2(100.f, 132.f), AR_COL32_WHITE);
				a->AddText(maxAverageFPSText, 16u, ArVec2(100.f, 148.f), AR_COL32_WHITE);
				a->AddText("--------Rendered In Foreground--------", 16u, ArVec2(100.f, 164.f), AR_COL32_WHITE);
				float y = 180.f;
				for (const auto& layer : ArGui::GetGraphicManager().GetLayers())
				{
					ArString name =
						layer == ArGui::GetGraphicManager().GetBackgroundLayer() ? "Background" :
						layer == ArGui::GetGraphicManager().GetDefaultLayer() ? "Default" :
						layer == ArGui::GetGraphicManager().GetForegroundLayer() ? "Foreground" :
						"Unknown";
					name = fmt::format("({})Layer - {}: [count:{}]", (void*)&layer, name, layer->GetElementCount());
					a->AddText(name, 16u, ArVec2(100.f, y), AR_COL32_WHITE);
					y += 16.f;
				}

				a->AddLine(ArVec2(100.f, 300.f), ArVec2(200.f, 300.f), AR_COL32_WHITE);
				a->AddRect(ArRect(100.f, 350.f, 200.f, 400.f), AR_COL32_WHITE);
				a->AddRectFilled(ArRect(100.f, 410.f, 200.f, 460.f), AR_COL32(255.f, 0.f, 0.f, 255.f));
				a->AddRectFilledMultiColor(ArRect(100.f, 470.f, 200.f, 520.f), AR_COL32(255.f, 0.f, 0.f, 255.f), AR_COL32(0.f, 255.f, 0.f, 255.f), AR_COL32(0.f, 0.f, 255.f, 255.f), AR_COL32(255.f, 255.f, 0.f, 255.f));
				a->AddQuadFilled(ArVec2(100.f, 530.f), ArVec2(200.f, 530.f), ArVec2(200.f, 580.f), ArVec2(100.f, 580.f), AR_COL32(255.f, 0.f, 0.f, 255.f));
				a->AddTriangleFilled(ArVec2(100.f, 590.f), ArVec2(200.f, 590.f), ArVec2(150.f, 640.f), AR_COL32(255.f, 0.f, 0.f, 255.f));
				a->AddCircle(ArVec2(150.f, 700.f), 50.f, AR_COL32(255.f, 0.f, 0.f, 255.f), 1.f);
				a->AddCircleFilled(ArVec2(150.f, 800.f), 50.f, AR_COL32(255.f, 0.f, 0.f, 255.f));

				a->AddLine(ArVec2(300.f, 300.f), ArVec2(400.f, 300.f), AR_COL32_WHITE, 5.f);
				a->AddRect(ArRect(300.f, 350.f, 400.f, 400.f), AR_COL32_WHITE, 5.f, ArVec4(10.f, 0.f, 10.f, 0.f));
				a->AddRectFilled(ArRect(300.f, 410.f, 400.f, 460.f), AR_COL32(255.f, 0.f, 0.f, 255.f), ArVec4(10.f, 10.f, 10.f, 10.f));
				a->AddRectFilledMultiColor(ArRect(300.f, 470.f, 400.f, 520.f), AR_COL32(255.f, 0.f, 0.f, 255.f), AR_COL32(0.f, 255.f, 0.f, 255.f), AR_COL32(0.f, 0.f, 255.f, 255.f), AR_COL32(255.f, 255.f, 0.f, 255.f));
				a->AddQuadFilled(ArVec2(300.f, 530.f), ArVec2(400.f, 530.f), ArVec2(400.f, 580.f), ArVec2(300.f, 580.f), AR_COL32(255.f, 0.f, 0.f, 255.f));
				a->AddTriangleFilled(ArVec2(300.f, 590.f), ArVec2(400.f, 590.f), ArVec2(350.f, 640.f), AR_COL32(255.f, 0.f, 0.f, 255.f));
				a->AddCircle(ArVec2(350.f, 700.f), 50.f, AR_COL32(255.f, 0.f, 0.f, 255.f), 3.f);
			}
			ArGui::EndFrame();
		}

		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

		ArGui::Present();

		g_pSwapChain->Present(0, 0);
	}

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	ArGui::Shutdown();
	return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ArWin32WndProc(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
