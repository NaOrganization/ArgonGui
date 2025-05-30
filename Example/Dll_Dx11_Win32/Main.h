﻿#pragma once
#include <d3d11.h>
#include <tchar.h>
#include <math.h>
#include <tuple>
#include <iostream>

#include <ArgonGui.h>
#include <Backends/ArgonWin32Platform.h>
#include <Backends/ArgonDx11Renderer.h>
#include <Backends/ArgonFreeTypeGlyphParser.h>

#include <fmt/core.h>

#include <detours/detours.h>

#include "../Demo.h"

#pragma comment(lib, "d3d11.lib")

static HMODULE selfModule = NULL;
static bool unloaded = false;