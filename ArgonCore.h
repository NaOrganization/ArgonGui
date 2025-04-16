#pragma once
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
//  !. Forward declarations: ArgonCore
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
//  !. Forward declarations: ArgonInput
// ---------------------------------------------------------- //

enum class ArKeyCode;
enum class ArMouseButton;
enum class ArMouseWheel;
enum class ArMouseInputSource;
enum class ArGamepadButton;
enum class ArInputDevice;
enum class ArInputAction;

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

class IArPlatformConfig;
class IArgonPlatform;

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
//  !. Forward declarations: ArgonRender
// ---------------------------------------------------------- //

class ArVertex;
class ArRenderBatch;
class ArRenderListSharedData;
class ArRenderList;
class ArTextureAtlas;
class ArFontFace;
class ArTextureManager;
class ArgonRenderManager;

class IArgonGlyphParser;
class IArRendererConfig;
class IArgonRenderer;

// [ALIAS] [ArgonRenderSystem] - Standard texture type
using ArTextureID = void*;
// [ALIAS] [ArgonRenderSystem] - Standard shader type
using ArCustomShaderID = void*;

// ---------------------------------------------------------- //
//  !. Forward declarations: ArgonContext
// ---------------------------------------------------------- //

class ArgonContextStatus;
class ArgonContext;

// ----------------------------------------------------------- //
//  !. Main modules includes
// ----------------------------------------------------------- //

#include "ArgonMath.h"
#include "ArgonInput.h"
#include "ArgonRender.h"
#include "ArgonGraphic.h"
#include "ArgonContext.h"

// ----------------------------------------------------------- //
//  !. Main modules namespace
// ----------------------------------------------------------- //

namespace ArGui
{
	inline ArgonGraphicManager& GetGraphicManager() noexcept { return ArgonContext::GetInstance().graphicManager; }

	inline ArgonRenderManager& GetRenderManager() noexcept { return ArgonContext::GetInstance().renderManager; }

	inline ArgonInputManager& GetInputManager() noexcept { return ArgonContext::GetInstance().inputManager; }

	inline ArgonContext& GetContext() noexcept { return ArgonContext::GetInstance(); }

	inline bool Initialize() { return ArgonContext::GetInstance().Initialize(); }

	inline void Shutdown() { ArgonContext::GetInstance().Shutdown(); }

	inline bool SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& platformConfig) { return ArgonContext::GetInstance().SetPlatform(platform, platformConfig); }

	inline bool SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& rendererConfig) { return ArgonContext::GetInstance().SetRenderer(renderer, rendererConfig); }

	inline bool SetGlyphParser(IArgonGlyphParser* glyphParser) { return ArgonContext::GetInstance().SetGlyphParser(glyphParser); }

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