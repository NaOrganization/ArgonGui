#pragma once

// ---------------------------------------------------------- //
//  !.Argon context
// ---------------------------------------------------------- //

// [STRUCT] [ArgonContext] - The running status of ArgonGui
class ArgonContextStatus final
{
public:
	bool started = false;
	bool running = false;
	bool paused = false;
	ArTimePoint startTime = ArTimePoint();
	ArTimePoint currentTime = ArTimePoint();
	ArTimePoint lastFrameUpdatedTime = ArTimePoint();
	ArDuration deltaTime = ArDuration();

	ArgonContextStatus() {}
	ArgonContextStatus(const ArgonContextStatus&) = delete;
	ArgonContextStatus& operator=(const ArgonContextStatus&) = delete;
	~ArgonContextStatus() {}

	void Awake();

	void StartFrame();

	void OnDestroy();
};

// [CLASS] [ArgonContext] - The main context of ArgonGui
class ArgonContext final
{
private:
	std::mutex contextMutex = {};
public:
	IArgonPlatform* platform = nullptr;
	IArgonRenderer* renderer = nullptr;
	ArgonInputManager inputManager = ArgonInputManager();
	ArgonRenderManager renderManager = ArgonRenderManager();
	ArgonGraphicManager graphicManager = ArgonGraphicManager();
	ArgonContextStatus contextStatus = ArgonContextStatus();

private:
	ArgonContext() {}
public:
	ArgonContext(const ArgonContext&) = delete;
	ArgonContext& operator=(const ArgonContext&) = delete;
	~ArgonContext() {}

	static ArgonContext& GetInstance();

	bool SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& platformConfig);
	bool SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& rendererConfig);
	bool SetGlyphParser(IArgonGlyphParser* glyphParser);

	bool Initialize();
	void Shutdown();
	void PauseRender() { contextStatus.paused = true; }
	void ContinueRender() { contextStatus.paused = false; }
	bool IsRunning() const { return contextStatus.running; }
	bool IsStarted() const { return contextStatus.started; }
	bool IsAllInterfacesReady() const { return platform && renderer && renderManager.textureManager.glyphParser; }
	float GetDeltaTime() const { return std::chrono::duration<float>(contextStatus.deltaTime).count(); }

	bool StartFrame();
	void EndFrame();
	bool FrameUpdate(); // StartFrame + EndFrame

	void Present();
};