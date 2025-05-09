#include "Main.h"

// Main code
int main(int, char**)
{
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Example", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

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

	g_pRenderer = new ArgonDx9Renderer();

	if (!ArGui::SetRenderer(g_pRenderer, ArDx9RendererConfig(g_pd3dDevice)))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	if (!ArGui::SetGlyphParser(new ArgonFreeTypeGlyphParser(), {}))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		ArGui::Shutdown();
		return 1;
	}

	// english
	ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\CascadiaCode.ttf");
	// chinese
	ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\simhei.ttf");
	// emoji
	ArGui::GetRenderManager().AddFontFromPath("C:\\Windows\\Fonts\\seguiemj.ttf");

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

		if (g_DeviceLost)
		{
			HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
			if (hr == D3DERR_DEVICELOST)
			{
				::Sleep(10);
				continue;
			}
			if (hr == D3DERR_DEVICENOTRESET)
				ResetDevice();
			g_DeviceLost = false;
		}

		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			g_d3dpp.BackBufferWidth = g_ResizeWidth;
			g_d3dpp.BackBufferHeight = g_ResizeHeight;
			g_ResizeWidth = g_ResizeHeight = 0;
			ResetDevice();
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
					name = fmt::format("({})Layer - {}: [count:{}] [focus:{}] [hover:{}]", (void*)&layer, name, layer->GetElementCount(), (void*)layer->focusingElement, (void*)layer->hoveringElement);
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

		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		HRESULT hr = g_pd3dDevice->BeginScene();
		if (hr == S_OK)
		{
			ArGui::Present();
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
		if (result == D3DERR_DEVICELOST)
			g_DeviceLost = true;
	}

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	ArGui::Shutdown();
	return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
	g_pRenderer->ReleaseRendererObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	g_pRenderer->CreateRendererObjects();
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
