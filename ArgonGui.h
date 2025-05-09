#pragma once
// ArgonGui - A lightweight, cross-platform, untrammeled GUI framework that doesn't ask for much—just your heart.
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
// © 2025 by [NaOrganization](https://github.com/NaOrganization) & All ArgonGui Contributors
// Version: v1.1.1
// Repository: https://github.com/NaOrganization/ArgonGui
// License: Apache License 2.0

#define ARGONGUI_VERSION "v1.1.1"

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
class ArGamepadConnectionEventData;
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
	ArGamepadConnectionEventData,
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

enum class ArLayerAdditionPriority;
enum class ArTimerCompMode;
enum class ArFlexLayoutDirection;
enum class ArFlexLayoutJustifyContent;
enum class ArFlexLayoutAlignItems;
enum class ArFlexLayoutWrap;
enum class ArFlexLayoutAlignContent;

class IArGraphicElementComponent;

class ArGraphicRenderList;
class ArGraphicBoxModel;
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
	inline ArgonInputManager& GetInputManager() noexcept { return ArgonContext::GetInstance().inputManager; }

	inline ArgonRenderManager& GetRenderManager() noexcept { return ArgonContext::GetInstance().renderManager; }

	inline ArgonGraphicManager& GetGraphicManager() noexcept { return ArgonContext::GetInstance().graphicManager; }

	inline ArgonGraphicThemeManager& GetThemeManager() noexcept { return ArgonContext::GetInstance().themeManager; }


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
