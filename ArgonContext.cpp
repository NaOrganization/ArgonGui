#include "ArgonGui.h"

ArgonContext* argonContextInstance = nullptr;

void ArgonContextStatus::Awake()
{
	startTime = ArHelp::GetTimePoint();
	currentFrameStartedTime = startTime;
	started = true;
}

void ArgonContextStatus::StartFrame()
{
	lastFrameUpdatedTime = currentFrameStartedTime;
	currentFrameStartedTime = ArHelp::GetTimePoint();
	deltaTime = currentFrameStartedTime - lastFrameUpdatedTime;
	running = true;
}

void ArgonContextStatus::OnDestroy()
{
	started = false;
	running = false;
}

ArgonContext& ArgonContext::GetInstance()
{
	if (!argonContextInstance)
		argonContextInstance = new ArgonContext();
	return *argonContextInstance;
}

bool ArgonContext::SetPlatform(IArgonPlatform* platform, const IArPlatformConfig& config)
{
	if (!platform)
		return false;
	if (!platform->Awake(config))
		return false;
	this->platform = platform;
	return true;
}

bool ArgonContext::SetRenderer(IArgonRenderer* renderer, const IArRendererConfig& config)
{
	if (!renderer)
		return false;
	if (!renderer->Awake(config))
		return false;
	this->renderer = renderer;
	return true;
}

bool ArgonContext::SetGlyphParser(IArgonGlyphParser* glyphParser, const IArGlyphParserConfig& config)
{
	if (!glyphParser)
		return false;
	if (!glyphParser->Awake(config))
		return false;
	this->glyphParser = glyphParser;
	return true;
}

bool ArgonContext::Awaken()
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
	renderManager.OnDestroy(renderer);

	if (platform)
	{
		delete platform;
		platform = nullptr;
	}
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}
	if (glyphParser)
	{
		delete glyphParser;
		glyphParser = nullptr;
	}
	contextStatus.OnDestroy();
}

bool ArgonContext::StartFrame()
{
	if (!IsAllInterfacesReady() ||
		!platform->IsRunning() || !renderer->IsRunning())
	{
		contextStatus.running = false;
		return false;
	}

	contextStatus.StartFrame();

	platform->StartFrame(inputManager);

	inputManager.StartFrame();

	renderer->StartFrame(renderManager);

	renderManager.StartFrame(glyphParser);

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