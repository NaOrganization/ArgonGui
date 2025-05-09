#pragma once
#include "../ArgonGui.h"
#include <d3d9.h>

class ArDx9RendererConfig final : public IArRendererConfig
{
public:
	IDirect3DDevice9* device = nullptr;

	ArDx9RendererConfig(IDirect3DDevice9* device) : device(device) {}
};

class ArgonDx9Renderer final : public IArgonRenderer
{
public:
	IDirect3DDevice9* device = nullptr;
private:
	IDirect3DVertexBuffer9* vertexBuffer = nullptr;
	size_t vertexBufferSize = 0x1400 * sizeof(ArVertex);
	IDirect3DIndexBuffer9* indexBuffer = nullptr;
	size_t indexBufferSize = 0x2800 * sizeof(uint16_t);
	bool supportRGBA = false;
	std::vector<IDirect3DTexture9*> textures = {};
public:
	~ArgonDx9Renderer();

	ArString Name() const override { return "DirectX 9"; }

	bool Awake(const IArRendererConfig& config) override;

	void StartFrame(const ArgonRenderManager& renderManager) override;

	void EndFrame(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) override {}

	ArTextureID CreateTexture(ArIntVec2 size, const void* pixels) override;

	ArShaderID CreateCustomShader(const IArRenderCustomCreateConfig& config) override { return nullptr; }

	void SetCurrentVertexShader(ArShaderID shader) override {}

	bool SetVertexShaderConstantBuffer(const void* data, size_t size) override { return false; }

	void SetCurrentPixelShader(ArShaderID shader) override {}

	bool SetPixelShaderConstantBuffer(const void* data, size_t size) override { return false; }

	void ReleaseTexture(ArTextureID texture) override;

	void ReleaseCustomShader(ArShaderID shader) override;

	void Present(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) override;

	bool IsRunning() const override;

	bool CreateRendererObjects();

	void ReleaseRendererObjects();
private:
	IDirect3DTexture9* CreateTextureInternal(ArIntVec2 size, const void* pixels, DWORD usage);
};
