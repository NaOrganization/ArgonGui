#pragma once

// ----------------------------------------------------------- //
//  [ARGON] Context - Base management of ArgonGui
// ----------------------------------------------------------- //

// [STRUCT] [ArgonContext] - The running status of ArgonGui
class ArgonContextStatus final
{
public:
	bool started = false;
	bool running = false;
	ArTimePoint startTime = ArTimePoint();
	ArTimePoint currentFrameStartedTime = ArTimePoint();
	ArTimePoint lastFrameUpdatedTime = ArTimePoint();
	ArDuration deltaTime = ArDuration();
public:
	ArgonContextStatus() {}
	ArgonContextStatus(const ArgonContextStatus&) = delete;
	ArgonContextStatus& operator=(const ArgonContextStatus&) = delete;
	~ArgonContextStatus() {}

	void Awake();

	void StartFrame();

	void OnDestroy();
};

// [CLASS] [ArgonContext] - The core element, managing all ArgonGui systems.
class ArgonContext final
{
public:
	ArgonContextStatus contextStatus = ArgonContextStatus();

	IArgonPlatform* platform = nullptr;
	IArgonRenderer* renderer = nullptr;
	IArgonGlyphParser* glyphParser = nullptr;

	ArgonInputManager inputManager = ArgonInputManager();
	ArgonRenderManager renderManager = ArgonRenderManager();
	ArgonGraphicManager graphicManager = ArgonGraphicManager();
	ArgonGraphicThemeManager themeManager = ArgonGraphicThemeManager();
private:
	ArgonContext() {}
	ArgonContext(const ArgonContext&) = delete;
	ArgonContext& operator=(const ArgonContext&) = delete;
	~ArgonContext() {}
public:
	static ArgonContext& GetInstance();
	
	bool SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& config);

	bool SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& config);

	bool SetGlyphParser(IArgonGlyphParser* glyphParser, const IArGlyphParserConfig& config);

	bool Awaken();

	void Shutdown();


	bool IsAllInterfacesReady() const { return platform && renderer && glyphParser; }

	float GetDeltaTime() const { return std::chrono::duration<float>(contextStatus.deltaTime).count(); }

	ArDuration GetDeltaTimeInArDuration() const { return contextStatus.deltaTime; }


	bool StartFrame();

	void EndFrame();

	// StartFrame + EndFrame
	bool FrameUpdate(); 


	void Present();
};