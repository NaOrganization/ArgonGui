#pragma once
#include "../ArgonCore.h"
#include <d3d11.h>

class ArDx11Buffer final
{
public:
	ID3D11Buffer* buffer = nullptr;
private:
	size_t currentSize = 0;
	const D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
	const uint32_t bindFlags = D3D11_BIND_VERTEX_BUFFER;
	const uint32_t cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	const uint32_t miscFlags = 0;
public:
	ArDx11Buffer(const size_t& defaultSize, const D3D11_USAGE& usage, const uint32_t& bindFlags, const uint32_t& cpuAccessFlags = D3D11_CPU_ACCESS_WRITE, const uint32_t& miscFlags = 0)
		: currentSize(defaultSize), usage(usage), bindFlags(bindFlags), cpuAccessFlags(cpuAccessFlags), miscFlags(miscFlags) {}

	~ArDx11Buffer() { Release(); }

	bool IsCreated() const { return buffer != nullptr; }

	size_t Size() const { return currentSize; }

	bool Create(ID3D11Device* device);

	void Release();

	bool Resize(size_t newSize, ID3D11Device* device);

	bool Expand(size_t size, ID3D11Device* device);
};

class ArDx11RendererConfig final : public IArRendererConfig
{
public:
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	ArDx11RendererConfig(IDXGISwapChain* swapChain,ID3D11Device* device, ID3D11DeviceContext* deviceContext) : swapChain(swapChain), device(device), deviceContext(deviceContext) {}
};

class ArDx11RenderCustomCreateConfig final : public IArRenderCustomCreateConfig
{
public:
	enum class ShaderType
	{
		Vertex,
		Pixel,
		None
	};
public:
	ShaderType shaderType = ShaderType::None;
	std::string shaderCode = "";
	std::string shaderModel = "";
public:
	ArDx11RenderCustomCreateConfig(ShaderType shaderType, const std::string& shaderCode, const std::string& shaderModel)
		: shaderType(shaderType), shaderCode(shaderCode), shaderModel(shaderModel) {}
};

class ArgonDx11Renderer final : public IArgonRenderer
{
public:
	using ConfigType = ArDx11RendererConfig;
public:
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
private:
	ArDx11Buffer vertexBuffer = ArDx11Buffer(0x1400 * sizeof(ArVertex), D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER);
	ArDx11Buffer indexBuffer = ArDx11Buffer(0x2800 * sizeof(uint16_t), D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER);
	ArDx11Buffer vertexConstantBuffer = ArDx11Buffer(sizeof(float[4][4]), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);
	ArDx11Buffer vertexShaderParmsBuffer = ArDx11Buffer(32, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);
	ArDx11Buffer pixelShaderParmsBuffer = ArDx11Buffer(32, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);

	ID3D11PixelShader* defualtPixelShader = nullptr;
	ID3D11VertexShader* defualtVertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11PixelShader* currentPixelShader = nullptr;
	ID3D11VertexShader* currentVertexShader = nullptr;

	std::unordered_map<ArTextureID, ID3D11Texture2D*> textures = {};
public:
	ArgonDx11Renderer() {}

	std::string Name() const override { return "Dx11"; }

	bool Awake(const IArRendererConfig& config) override;

	void StartFrame(const ArgonRenderManager& renderManager) override;

	void EndFrame(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) override;

	void OnDestroy() override;

	ArTextureID CreateTexture(ArIntVec2 size, const void* pixels) override;

	ArShaderID CreateCustomShader(const IArRenderCustomCreateConfig& config) override;

	void SetCurrentVertexShader(ArShaderID shader) override;

	bool SetVertexShaderConstantBuffer(const void* data, size_t size) override;

	void SetCurrentPixelShader(ArShaderID shader) override;

	bool SetPixelShaderConstantBuffer(const void* data, size_t size) override;

	void ReleaseTexture(ArTextureID texture) override;

	void ReleaseCustomShader(ArShaderID shader) override;

	void Present(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) override;

	bool IsRunning() const override;
private:
	bool CreateRendererObjects();

	bool CreateVertexShader();

	bool CreatePixelShader();

	bool CreateStates();

	void ReleaseRendererObjects();

	ArTextureID CreateTextureInternal(ArIntVec2 size, const void* pixels, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, bool recordResource);
};
