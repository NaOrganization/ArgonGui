#pragma once
// ArgonGui - A lightweight GUI library for C++
// 
//       ___           ___           ___           ___           ___           ___           ___                 
//      /\  \         /\  \         /\  \         /\  \         /\__\         /\  \         /\__\          ___   
//     /::\  \       /::\  \       /::\  \       /::\  \       /::|  |       /::\  \       /:/  /         /\  \  
//    /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:|:|  |      /:/\:\  \     /:/  /          \:\  \ 
//   /::\~\:\  \   /::\~\:\  \   /:/  \:\  \   /:/  \:\  \   /:/|:|  |__   /:/  \:\  \   /:/  /  ___      /::\__\
//  /:/\:\ \:\__\ /:/\:\ \:\__\ /:/__/_\:\__\ /:/__/ \:\__\ /:/ |:| /\__\ /:/__/_\:\__\ /:/__/  /\__\  __/:/\/__/
//  \/__\:\/:/  / \/_|::\/:/  / \:\  /\ \/__/ \:\  \ /:/  / \/__|:|/:/  / \:\  /\ \/__/ \:\  \ /:/  / /\/:/  /   
//       \::/  /     |:|::/  /   \:\ \:\__\    \:\  /:/  /      |:/:/  /   \:\ \:\__\    \:\  /:/  /  \::/__/    
//       /:/  /      |:|\/__/     \:\/:/  /     \:\/:/  /       |::/  /     \:\/:/  /     \:\/:/  /    \:\__\    
//      /:/  /       |:|  |        \::/  /       \::/  /        /:/  /       \::/  /       \::/  /      \/__/    
//      \/__/         \|__|         \/__/         \/__/         \/__/         \/__/         \/__/                
//
// Copyright (c) 2025 ArgonCore
// Version: Beta 1.1.0
// Repository: https://github.com/NaOrganization/ArgonGui

#define ARGONGUI_VERSION "Beta 1.1.0"

#include <unordered_map>
#include <map>
#include <queue>
#include <variant>
#include <unordered_set>
#include <mutex>
#include <chrono>
#include <array>
#include <string>
#include <filesystem>

using namespace std::chrono_literals;

// ---------------------------------------------------------- //
//  [ARGON] Basic aliases
//		- Standard types and aliases in ArgonGui
// ---------------------------------------------------------- //

// [ALIAS] [ArgonCore] - Standard time duration
using ArDuration = std::chrono::nanoseconds;
// [ALIAS] [ArgonCore] - Standard time point
using ArTimePoint = std::chrono::time_point<std::chrono::system_clock, ArDuration>;
// [ALIAS] [ArgonCore] - Standard id type
using ArGuiID = uint32_t;
// [ALIAS] [ArgonCore] - Standard character type
using ArChar = char;
// [ALIAS] [ArgonCore] - Standard string type
using ArString = std::basic_string<ArChar>;
// [ALIAS] [ArgonCore] - Standard string view type
using ArStringView = std::basic_string_view<ArChar>;

// ---------------------------------------------------------- //
//  [ARGON] [ArgonInput] Forward declarations
// ---------------------------------------------------------- //

enum class ArKeyCode;
enum class ArMouseButton;
enum class ArMouseWheel;
enum class ArMouseInputSource;
enum class ArGamepadButton;
enum class ArInputDevice;
enum class ArInputAction;

class IArPlatformConfig;
class IArgonPlatform;

class ArMouseEventData;
class ArMouseMoveEventData;
class ArMouseButtonEventData;
class ArMouseWheelEventData;
class ArKeyboardEventData;
class ArGamepadEventData;
class ArGamepadButtonEventData;
class ArGamepadLeftStickEventData;
class ArGamepadRightStickEventData;
class ArGamepadLeftTriggerEventData;
class ArGamepadRightTriggerEventData;
class ArDisplayEventData;
class ArInputEvent;
class ArButtonState;
class ArInputDeviceState;
class ArMouseState;
class ArKeyboardState;
class ArGamepadState;
class ArgonInputManager;

// [VARIANT] [ArgonInputSystem] - Union structure for data of the event
using ArInputEventData = std::variant<
	ArMouseMoveEventData,
	ArMouseButtonEventData,
	ArMouseWheelEventData,
	ArKeyboardEventData,
	ArGamepadButtonEventData,
	ArGamepadLeftStickEventData,
	ArGamepadRightStickEventData,
	ArGamepadLeftTriggerEventData,
	ArGamepadRightTriggerEventData,
	ArDisplayEventData>;

// ---------------------------------------------------------- //
//  [ARGON] [ArgonRender] Forward declarations
// ---------------------------------------------------------- //

class IArgonGlyphParser;
class IArRendererConfig;
class IArRenderCustomCreateConfig;
class IArgonRenderer;

class ArVertex;
class ArRenderBatch;
class ArRenderListSharedData;
class ArRenderList;
class ArTextureLand;
class ArGlyphKey;
class ArGlyphInfo;
class ArFont;
class ArgonRenderManager;

// [ALIAS] [ArgonRenderSystem] - Standard texture type
using ArTextureID = void*;
// [ALIAS] [ArgonRenderSystem] - Standard shader type
using ArShaderID = void*;

// ----------------------------------------------------------- //
//  [ARGON] [ArgonGraphic] Forward declarations
// ----------------------------------------------------------- //

class IArGraphicComponent;

class ArGraphicRenderList;
class ArGraphicBoundingBox;
class ArGraphicElement;
class ArGraphicLayer;
class ArGraphicPrimRenderListElement;
class ArgonGraphicManager;

// ---------------------------------------------------------- //
//  [ARGON] [ArgonContext] Forward declarations
// ---------------------------------------------------------- //

class ArgonContextStatus;
class ArgonContext;

// ----------------------------------------------------------- //
//  [ARGON] Modules
// ----------------------------------------------------------- //

#include "ArgonTemplate.h"
#include "ArgonMath.h"
#include "ArgonInput.h"
#include "ArgonRender.h"
#include "ArgonGraphic.h"
#include "ArgonContext.h"

// ----------------------------------------------------------- //
//  [ARGON] Main api namespace
// ----------------------------------------------------------- //

namespace ArGui
{
	inline ArgonGraphicManager& GetGraphicManager() noexcept { return ArgonContext::GetInstance().graphicManager; }

	inline ArgonRenderManager& GetRenderManager() noexcept { return ArgonContext::GetInstance().renderManager; }

	inline ArgonInputManager& GetInputManager() noexcept { return ArgonContext::GetInstance().inputManager; }

	inline ArgonContext& GetContext() noexcept { return ArgonContext::GetInstance(); }

	inline bool Initialize() { return ArgonContext::GetInstance().Awaken(); }

	inline void Shutdown() { ArgonContext::GetInstance().Shutdown(); }

	inline bool IsRunning() { return ArgonContext::GetInstance().contextStatus.running; }

	inline bool SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& config) { return ArgonContext::GetInstance().SetPlatform(platform, config); }

	inline bool SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& config) { return ArgonContext::GetInstance().SetRenderer(renderer, config); }

	inline bool SetGlyphParser(IArgonGlyphParser* glyphParser, const IArGlyphParserConfig& config) { return ArgonContext::GetInstance().SetGlyphParser(glyphParser, config); }

	inline bool StartFrame() { return ArgonContext::GetInstance().StartFrame(); }

	inline void EndFrame() { ArgonContext::GetInstance().EndFrame(); }

	inline void FrameUpdate() { ArgonContext::GetInstance().FrameUpdate(); }

	inline void Present() { ArgonContext::GetInstance().Present(); }
}

namespace ArHelp
{
	ArTimePoint GetTimePoint();
	ArGuiID Hash(ArStringView string, ArGuiID seed = GetTimePoint().time_since_epoch().count());

	namespace Utf8
	{
		std::vector<uint32_t> DecodeToCodepoints(ArStringView str);
	}

	namespace Random
	{
		float GetFloat(float min, float max);
	}
}
