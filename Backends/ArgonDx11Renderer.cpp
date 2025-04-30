#include "ArgonDx11Renderer.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler")

bool ArDx11Buffer::Create(ID3D11Device* device)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = usage;
	desc.ByteWidth = (UINT)currentSize;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = miscFlags;
	return device->CreateBuffer(&desc, nullptr, &buffer) == S_OK;
}

void ArDx11Buffer::Release()
{
	if (!buffer)
		return;
	buffer->Release();
	buffer = nullptr;
}

bool ArDx11Buffer::Resize(size_t newSize, ID3D11Device* device)
{
	if (newSize == currentSize)
		return true;
	Release();
	currentSize = newSize;
	return Create(device);
}

bool ArDx11Buffer::Expand(size_t size, ID3D11Device* device)
{
	if (size == 0)
		return true;
	Release();
	currentSize += size;
	return Create(device);
}

bool ArgonDx11Renderer::Awake(const IArRendererConfig& config)
{
	const ArDx11RendererConfig& dx11Config = static_cast<const ArDx11RendererConfig&>(config);
	swapChain = dx11Config.swapChain;
	device = dx11Config.device;
	deviceContext = dx11Config.deviceContext;
	device->AddRef();
	deviceContext->AddRef();

	if (!CreateRendererObjects())
	{
		ReleaseRendererObjects();
		return false;
	}

	return true;
}

void ArgonDx11Renderer::StartFrame(const ArgonRenderManager& renderManager)
{
	for (auto& land : renderManager.lands)
	{
		if (land.Ready())
			continue;
		land.textureId = CreateTextureInternal(land.size, land.pixels, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0, true);
	}
}

void ArgonDx11Renderer::EndFrame(const ArgonRenderManager& renderManager, const ArDisplayState& displayState)
{
	// Resize buffers if needed
	{
		if (vertexBuffer.Size() < renderManager.totalVertexCount * sizeof(ArVertex))
			vertexBuffer.Expand(0x1400 * sizeof(ArVertex), device);
		if (indexBuffer.Size() < renderManager.totalIndexCount * sizeof(uint16_t))
			indexBuffer.Expand(0x2800 * sizeof(uint16_t), device);
	}

	// Update vertex and index buffers
	{
		D3D11_MAPPED_SUBRESOURCE vtxResource = {}, idxResource = {};
		if (deviceContext->Map(vertexBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtxResource) != S_OK)
			return;
		if (deviceContext->Map(indexBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idxResource) != S_OK)
			return;
		{
			ArVertex* vtxDst = (ArVertex*)vtxResource.pData;
			uint16_t* idxDst = (uint16_t*)idxResource.pData;
			for (auto& renderList : renderManager.renderLists)
			{
				std::copy(renderList->vertices.data(), renderList->vertices.data() + renderList->vertices.size(), vtxDst);
				std::copy(renderList->indices.data(), renderList->indices.data() + renderList->indices.size(), idxDst);

				vtxDst += renderList->vertices.size();
				idxDst += renderList->indices.size();
			}
		}
		deviceContext->Unmap(vertexBuffer.buffer, 0);
		deviceContext->Unmap(indexBuffer.buffer, 0);
	}

	// Update constant buffer
	{
		const float L = 0.0f;
		const float R = displayState.size.x;
		const float T = 0.0f;
		const float B = displayState.size.y;
		float mvp[4][4] =
		{
			{ 2.0f / (R - L),		0.0f,					0.0f,				0.0f },
			{ 0.0f,					2.0f / (T - B),			0.0f,				0.0f },
			{ 0.0f,					0.0f,					0.5f,				0.0f },
			{ (R + L) / (L - R),	(T + B) / (B - T),		0.5f,				1.0f },
		};
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (deviceContext->Map(vertexConstantBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
			return;
		memcpy(mappedResource.pData, mvp, sizeof(mvp));
		deviceContext->Unmap(vertexConstantBuffer.buffer, 0);
	}

	// Update font lands
	{
		for (auto& land : renderManager.lands)
		{
			if (!land.Ready() || !land.dirty)
				continue;

			ID3D11Texture2D* texture = textures[land.textureId];
			if (!texture)
				continue;
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			if (deviceContext->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
				continue;
			std::copy(land.pixels, land.pixels + (land.size.x * land.size.y), (uint32_t*)mappedResource.pData);
			deviceContext->Unmap(texture, 0);

			land.dirty = false;
		}
	}
}

void ArgonDx11Renderer::OnDestroy()
{
	vertexBuffer.Release();
	indexBuffer.Release();
	vertexConstantBuffer.Release();
	ReleaseRendererObjects();
	if (device)
	{
		device->Release();
		device = nullptr;
	}
	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}
	for (auto& texture : textures)
	{
		texture.second->Release();
	}
}

ArTextureID ArgonDx11Renderer::CreateTexture(ArIntVec2 size, const void* pixels)
{
	return CreateTextureInternal(size, pixels, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false);
}

ArShaderID ArgonDx11Renderer::CreateCustomShader(const IArRenderCustomCreateConfig& config)
{
	const ArDx11RenderCustomCreateConfig& dx11Config = static_cast<const ArDx11RenderCustomCreateConfig&>(config);
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	D3DCompile(dx11Config.shaderCode.c_str(), dx11Config.shaderCode.size(), nullptr, nullptr, nullptr, "main", dx11Config.shaderModel.c_str(), 0, 0, &shaderBlob, &errorBlob);
	if (errorBlob)
	{
		const char* errorMessage = (const char*)errorBlob->GetBufferPointer();
 		errorBlob->Release();
		return nullptr;
	}
	if (dx11Config.shaderType == ArDx11RenderCustomCreateConfig::ShaderType::Vertex)
	{
		ID3D11VertexShader* vertexShader = nullptr;
		if (device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader) != S_OK)
		{
			shaderBlob->Release();
			return nullptr;
		}
		return vertexShader;
	}
	else
	{
		ID3D11PixelShader* pixelShader = nullptr;
		if (device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader) != S_OK)
		{
			shaderBlob->Release();
			return nullptr;
		}
		return pixelShader;
	}
	return nullptr;
}

void ArgonDx11Renderer::SetCurrentVertexShader(ArShaderID shader)
{
	if (!shader)
	{
		currentVertexShader = defualtVertexShader;
		return;
	}
	currentVertexShader = (ID3D11VertexShader*)shader;
}

bool ArgonDx11Renderer::SetVertexShaderConstantBuffer(const void* data, size_t size)
{
	if (!data || size == 0)
		return false;

	if (vertexShaderParmsBuffer.Size() < size || !vertexShaderParmsBuffer.IsCreated())
	{
		if (!vertexShaderParmsBuffer.Expand(size, device))
			return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (deviceContext->Map(vertexShaderParmsBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
		return false;
	memcpy(mappedResource.pData, data, size);
	deviceContext->Unmap(vertexShaderParmsBuffer.buffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &vertexShaderParmsBuffer.buffer);
	return true;
}

void ArgonDx11Renderer::SetCurrentPixelShader(ArShaderID shader)
{
	if (!shader)
	{
		currentPixelShader = defualtPixelShader;
		return;
	}
	currentPixelShader = (ID3D11PixelShader*)shader;
}

bool ArgonDx11Renderer::SetPixelShaderConstantBuffer(const void* data, size_t size)
{
	if (!data || size == 0)
		return false;
	if (pixelShaderParmsBuffer.Size() < size || !pixelShaderParmsBuffer.IsCreated())
	{
		if (!pixelShaderParmsBuffer.Expand(size, device))
			return false;
	}
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (deviceContext->Map(pixelShaderParmsBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
		return false;
	memcpy(mappedResource.pData, data, size);
	deviceContext->Unmap(pixelShaderParmsBuffer.buffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &pixelShaderParmsBuffer.buffer);
	return true;
}

void ArgonDx11Renderer::ReleaseTexture(ArTextureID texture)
{
	if (texture)
	{
		((IUnknown*)texture)->Release();
		texture = nullptr;
	}
}

void ArgonDx11Renderer::ReleaseCustomShader(ArShaderID shader)
{
	if (shader)
	{
		((IUnknown*)shader)->Release();
		shader = nullptr;
	}
}

void ArgonDx11Renderer::Present(const ArgonRenderManager& renderManager, const ArDisplayState& displayState)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)displayState.size.x;
	vp.Height = (FLOAT)displayState.size.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);

	const UINT stride = sizeof(ArVertex);
	const UINT offset = 0;
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer.buffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer.buffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(currentVertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &vertexConstantBuffer.buffer);

	deviceContext->PSSetShader(currentPixelShader, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(depthStencilState, 0);

	deviceContext->RSSetState(rasterizerState);

	size_t globalIndexOffset = 0;
	size_t globalVertexOffset = 0;
	for (auto& renderList : renderManager.renderLists)
	{
		for (auto& batch : renderList->batches)
		{
			const D3D11_RECT clipRect = { (LONG)batch.scissor.min.x, (LONG)batch.scissor.min.y, (LONG)batch.scissor.max.x, (LONG)batch.scissor.max.y };
			deviceContext->RSSetScissorRects(1, &clipRect);

			if (batch.texture)
				deviceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&batch.texture);
			deviceContext->DrawIndexed((UINT)batch.indexCount, (UINT)(globalIndexOffset + batch.indexOffset), (INT)(globalVertexOffset));
		}
		globalIndexOffset += renderList->indices.size();
		globalVertexOffset += renderList->vertices.size();
	}
}

bool ArgonDx11Renderer::IsRunning() const
{
	if (!device || !deviceContext)
		return false;
	if (!vertexBuffer.IsCreated() || !indexBuffer.IsCreated())
		return false;
	if (!defualtVertexShader || !defualtPixelShader || !inputLayout)
		return false;
	if (!rasterizerState || !blendState || !depthStencilState || !samplerState)
		return false;
	return true;
}

bool ArgonDx11Renderer::CreateRendererObjects()
{
	if (!CreateVertexShader())
		return false;
	if (!CreatePixelShader())
		return false;
	if (!CreateStates())
		return false;
	return vertexBuffer.Create(device) && indexBuffer.Create(device);
}

bool ArgonDx11Renderer::CreateVertexShader()
{
	static const char* vertexShaderCode =
		"cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul(ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

	ID3DBlob* vertexShaderBlob;
	if (FAILED(D3DCompile(vertexShaderCode, strlen(vertexShaderCode), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vertexShaderBlob, nullptr)))
		return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
	if (device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &defualtVertexShader) != S_OK)
	{
		vertexShaderBlob->Release();
		return false;
	}

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC local_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(ArVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(ArVertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)offsetof(ArVertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (device->CreateInputLayout(local_layout, 3, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout) != S_OK)
	{
		vertexShaderBlob->Release();
		return false;
	}
	vertexShaderBlob->Release();

	// Create the constant buffer
	if (!vertexConstantBuffer.Create(device))
		return false;

	currentVertexShader = defualtVertexShader;

	return true;

}

bool ArgonDx11Renderer::CreatePixelShader()
{
	static const char* pixelShaderCode =
		"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

	ID3DBlob* pixelShaderBlob;
	if (FAILED(D3DCompile(pixelShaderCode, strlen(pixelShaderCode), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &pixelShaderBlob, nullptr)))
		return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
	if (device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &defualtPixelShader) != S_OK)
	{
		pixelShaderBlob->Release();
		return false;
	}
	pixelShaderBlob->Release();

	currentPixelShader = defualtPixelShader;

	return true;

}

bool ArgonDx11Renderer::CreateStates()
{
	// Create the blending setup
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		if (FAILED(device->CreateBlendState(&desc, &blendState)))
			return false;
	}

	// Create the rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		if (FAILED(device->CreateRasterizerState(&desc, &rasterizerState)))
			return false;
	}

	// Create depth-stencil State
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		if (FAILED(device->CreateDepthStencilState(&desc, &depthStencilState)))
			return false;
	}

	// Create sampler state
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0.f;
		desc.MaxLOD = 0.f;
		desc.MipLODBias = 0.f;
		if (FAILED(device->CreateSamplerState(&desc, &samplerState)))
			return false;
	}

	return true;

}

void ArgonDx11Renderer::ReleaseRendererObjects()
{
	if (defualtPixelShader)
	{
		defualtPixelShader->Release();
		defualtPixelShader = nullptr;
	}
	if (defualtVertexShader)
	{
		defualtVertexShader->Release();
		defualtVertexShader = nullptr;
	}
	if (inputLayout)
	{
		inputLayout->Release();
		inputLayout = nullptr;
	}
	if (rasterizerState)
	{
		rasterizerState->Release();
		rasterizerState = nullptr;
	}
	if (blendState)
	{
		blendState->Release();
		blendState = nullptr;
	}
	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
	if (samplerState)
	{
		samplerState->Release();
		samplerState = nullptr;
	}
}

ArTextureID ArgonDx11Renderer::CreateTextureInternal(ArIntVec2 size, const void* pixels, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, bool recordResource = true)
{
	if (size.x <= 0 || size.y <= 0)
		return nullptr;
	if (pixels == nullptr)
		return nullptr;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = size.x;
	desc.Height = size.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccessFlags;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = pixels;
	initData.SysMemPitch = size.x * 4;
	initData.SysMemSlicePitch = 0;

	ID3D11Texture2D* texture = nullptr;
	if (device->CreateTexture2D(&desc, &initData, &texture) != S_OK)
		return nullptr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* srv = nullptr;
	if (device->CreateShaderResourceView(texture, &srvDesc, &srv) != S_OK)
	{
		texture->Release();
		return nullptr;
	}
	if (recordResource)
		textures[srv] = texture;
	else
		texture->Release();
	return srv;
}
