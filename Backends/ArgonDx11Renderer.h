#pragma once
#include "../ArgonGui.h"
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
		: currentSize(defaultSize), usage(usage), bindFlags(bindFlags), cpuAccessFlags(cpuAccessFlags), miscFlags(miscFlags) {
	}

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

	ArDx11RendererConfig(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext) : swapChain(swapChain), device(device), deviceContext(deviceContext) {}
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
		: shaderType(shaderType), shaderCode(shaderCode), shaderModel(shaderModel) {
	}
};

class ArDx11RendererObjectBackups final
{
public:
	UINT scissorCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D11_RECT scissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
	UINT viewportCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	FLOAT blendFactor[4] = {};
	UINT sampleMask = 0;
	UINT stencilRef = 0;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11ShaderResourceView* pixelShaderResources = nullptr;
	ID3D11SamplerState* pixelShaderSamplers = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	UINT pixelShaderInstanceCount = 256;
	UINT vertexShaderInstanceCount = 256;
	UINT geometryShaderInstanceCount = 256;
	ID3D11ClassInstance* pixelShaderInstances[256] = {};
	ID3D11ClassInstance* vertexShaderInstances[256] = {};
	ID3D11ClassInstance* geometryShaderInstances[256] = {};
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	ID3D11Buffer* vertexConstantBuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	UINT vertexBufferStride = 0;
	UINT vertexBufferOffset = 0;
	ID3D11Buffer* indexBuffer = nullptr;
	DXGI_FORMAT indexBufferFormat = DXGI_FORMAT_UNKNOWN;
	UINT indexBufferOffset = 0;
	ID3D11InputLayout* inputLayout = nullptr;

	void Backup(ID3D11DeviceContext* deviceContext);

	void Restore(ID3D11DeviceContext* deviceContext);
};

class ArgonDx11Renderer final : public IArgonRenderer
{
public:
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
private:
	ArDx11Buffer vertexBuffer = ArDx11Buffer(0x1400 * sizeof(ArVertex), D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER);
	ArDx11Buffer indexBuffer = ArDx11Buffer(0x2800 * sizeof(uint16_t), D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER);
	ArDx11Buffer vertexConstantBuffer = ArDx11Buffer(sizeof(float[4][4]), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);
	ArDx11Buffer vertexShaderParamsBuffer = ArDx11Buffer(32, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);
	ArDx11Buffer pixelShaderParamsBuffer = ArDx11Buffer(32, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER);

	ID3D11PixelShader* defaultPixelShader = nullptr;
	ID3D11VertexShader* defaultVertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11PixelShader* currentPixelShader = nullptr;
	ID3D11VertexShader* currentVertexShader = nullptr;

	std::unordered_map<ArTextureID, ID3D11Texture2D*> textures = {};

	ArDx11RendererObjectBackups backups = {};
public:
	~ArgonDx11Renderer();

	ArString Name() const override { return "DirectX 11"; }

	bool Awake(const IArRendererConfig& config) override;

	void StartFrame(const ArgonRenderManager& renderManager) override;

	void EndFrame(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) override {}

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

	ArTextureID CreateTextureInternal(ArIntVec2 size, const void* pixels, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, bool recordResource);
};
