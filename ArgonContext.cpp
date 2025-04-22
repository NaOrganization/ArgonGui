#include "ArgonCore.h"

void ArgonContextStatus::Awake()
{
	startTime = ArHelp::GetTimePoint();
	currentTime = startTime;
	started = true;
}

void ArgonContextStatus::StartFrame()
{
	lastFrameUpdatedTime = currentTime;
	currentTime = ArHelp::GetTimePoint();
	deltaTime = currentTime - lastFrameUpdatedTime;
	running = true;
}

void ArgonContextStatus::OnDestroy()
{
	started = false;
	running = false;
}

ArgonContext& ArgonContext::GetInstance()
{
	static ArgonContext* instance = new ArgonContext();
	return *instance;
}

bool ArgonContext::SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& platformConfig)
{
	if (!platform)
		return false;
	if (!platform->Awake(std::forward<const IArPlatformConfig&>(platformConfig)))
		return false;
	this->platform = platform;
	return true;
}

bool ArgonContext::SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& rendererConfig)
{
	if (!renderer)
		return false;
	if (!renderer->Awake(std::forward<const IArRendererConfig&>(rendererConfig)))
		return false;
	this->renderer = renderer;
	return true;
}

bool ArgonContext::SetGlyphParser(IArgonGlyphParser* glyphParser)
{
	if (!glyphParser)
		return false;
	if (!glyphParser->Awake())
		return false;
	renderManager.textureManager.glyphParser = glyphParser;
	return true;
}

bool ArgonContext::Initialize()
{
	if (!IsAllInterfacesReady())
		return false;

	contextStatus.Awake();

	renderManager.Awake();

	graphicManager.Awake();
	return true;
}

void ArgonContext::Shutdown()
{
	if (platform)
	{
		platform->OnDestroy();
		platform = nullptr;
	}
	if (renderer)
	{
		renderer->OnDestroy();
		renderer = nullptr;
	}
	contextStatus.OnDestroy();
}

bool ArgonContext::StartFrame()
{
	if (!IsAllInterfacesReady() ||
		!platform->IsRunning() || !renderer->IsRunning() ||
		contextStatus.paused
		)
	{
		contextStatus.running = false;
		return false;
	}

	contextStatus.StartFrame();

	platform->StartFrame(inputManager);

	inputManager.StartFrame();

	renderer->StartFrame(renderManager);

	renderManager.StartFrame();

	graphicManager.StartFrame(*this);

	return true;
}

void ArgonContext::EndFrame()
{
	graphicManager.EndFrame(*this);

	inputManager.EndFrame();

	renderManager.EndFrame();

	renderer->EndFrame(renderManager, inputManager.GetDisplayState());
}

bool ArgonContext::FrameUpdate()
{
	if (!StartFrame())
		return false;

	EndFrame();
	return true;
}

void ArgonContext::Present()
{
	renderer->Present(renderManager, inputManager.GetDisplayState());

	renderManager.PostPresent();
}