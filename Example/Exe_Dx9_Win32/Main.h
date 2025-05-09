#pragma once
#include <d3d9.h>
#include <tchar.h>
#include <math.h>
#include <tuple>
#include <iostream>

#include <ArgonGui.h>
#include <Backends/ArgonWin32Platform.h>
#include <Backends/ArgonDx9Renderer.h>
#include <Backends/ArgonFreeTypeGlyphParser.h>

#include <fmt/core.h>

#include "../Demo.h"

#pragma comment(lib, "d3d9.lib")

// Data
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

static ArgonDx9Renderer* g_pRenderer = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
