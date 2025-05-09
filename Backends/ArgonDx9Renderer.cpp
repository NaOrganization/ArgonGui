#include "ArgonDx9Renderer.h"

#define DX_SAFE_RELEASE(p) if ((p)) { (p)->Release(); (p) = nullptr; }

struct CUSTOMVERTEX
{
	float    pos[3];
	D3DCOLOR col;
	float    uv[2];
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define COL_TO_DX9_ARGB(_COL)     (((_COL) & 0xFF00FF00) | (((_COL) & 0xFF0000) >> 16) | (((_COL) & 0xFF) << 16))

static bool CopyTextureRegion(bool convertFormat, IDirect3DTexture9* texture, const void* pixels, int width, int height, int pitch)
{
	if (!texture || !pixels)
		return false;
	D3DLOCKED_RECT lockedRect = {};
	if (texture->LockRect(0, &lockedRect, nullptr, 0) != D3D_OK)
		return false;
	for (int i = 0; i < height; ++i)
	{
		if (!convertFormat)
		{
			for (int j = 0; j < width; ++j)
			{
				uint32_t color = ((uint32_t*)pixels)[i * pitch + j];
				color = COL_TO_DX9_ARGB(color);
				((uint32_t*)lockedRect.pBits)[i * lockedRect.Pitch / 4 + j] = color;
			}
		}
		else
		{
			memcpy((uint8_t*)lockedRect.pBits + (i * lockedRect.Pitch) * 4, (const uint8_t*)pixels + (i * pitch), width * 4);
		}
	}
	texture->UnlockRect(0);

	return true;
}

ArgonDx9Renderer::~ArgonDx9Renderer()
{
	DX_SAFE_RELEASE(vertexBuffer);

	DX_SAFE_RELEASE(indexBuffer);

	DX_SAFE_RELEASE(device);

	for (auto& texture : textures)
	{
		if (texture)
		{
			texture->Release();
		}
	}
}

bool ArgonDx9Renderer::Awake(const IArRendererConfig& config)
{
	const ArDx9RendererConfig& dx9Config = static_cast<const ArDx9RendererConfig&>(config);
	device = dx9Config.device;
	if (!device)
		return false;
	device->AddRef();

	LPDIRECT3D9 pd3d = nullptr;
	if (device->GetDirect3D(&pd3d) != D3D_OK)
		return false;
	D3DDEVICE_CREATION_PARAMETERS param = {};
	D3DDISPLAYMODE mode = {};
	if (device->GetCreationParameters(&param) != D3D_OK || device->GetDisplayMode(0, &mode) != D3D_OK)
	{
		pd3d->Release();
		return false;
	}
	// Font texture should support linear filter, color blend and write to render-target
	supportRGBA = (pd3d->CheckDeviceFormat(param.AdapterOrdinal, param.DeviceType, mode.Format, D3DUSAGE_DYNAMIC | D3DUSAGE_QUERY_FILTER | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_TEXTURE, D3DFMT_A8B8G8R8)) == D3D_OK;
	pd3d->Release();

	if (!CreateRendererObjects())
	{
		return false;
	}
	return true;
}

void ArgonDx9Renderer::StartFrame(const ArgonRenderManager& renderManager)
{
	if (textures.size() != renderManager.lands.size())
	{
		textures.resize(renderManager.lands.size());
	}

	for (size_t i = 0; i < renderManager.lands.size(); i++)
	{
		auto& land = renderManager.lands[i];
		if (land.Ready() && textures[i])
			continue;
		land.textureId = textures[i] = CreateTextureInternal(land.size, land.pixels, D3DUSAGE_DYNAMIC);
	}
}

ArTextureID ArgonDx9Renderer::CreateTexture(ArIntVec2 size, const void* pixels)
{
	return CreateTextureInternal(size, pixels, D3DUSAGE_DYNAMIC);
}

void ArgonDx9Renderer::ReleaseTexture(ArTextureID texture)
{
	if (texture)
	{
		((IDirect3DTexture9*)texture)->Release();
	}
}

void ArgonDx9Renderer::ReleaseCustomShader(ArShaderID shader)
{
	if (shader)
	{
		((IUnknown*)shader)->Release();
	}
}

void ArgonDx9Renderer::Present(const ArgonRenderManager& renderManager, const ArDisplayState& displayState)
{
	IDirect3DStateBlock9* state_block = nullptr;
	if (device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
		return;
	if (state_block->Capture() < 0)
	{
		state_block->Release();
		return;
	}
	D3DMATRIX last_world, last_view, last_projection;
	device->GetTransform(D3DTS_WORLD, &last_world);
	device->GetTransform(D3DTS_VIEW, &last_view);
	device->GetTransform(D3DTS_PROJECTION, &last_projection);

	// Resize buffers if needed
	{
		if (vertexBufferSize < renderManager.totalVertexCount * sizeof(ArVertex))
		{
			DX_SAFE_RELEASE(vertexBuffer);
			vertexBufferSize = (renderManager.totalVertexCount + 0x1400) * sizeof(ArVertex);
			if (device->CreateVertexBuffer(vertexBufferSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vertexBuffer, nullptr) != D3D_OK)
			{
				vertexBuffer = nullptr;
				vertexBufferSize = 0;
			}
		}
		if (indexBufferSize < renderManager.totalIndexCount * sizeof(uint16_t))
		{
			DX_SAFE_RELEASE(indexBuffer);
			indexBufferSize = (renderManager.totalIndexCount + 0x2800) * sizeof(uint16_t);
			if (device->CreateIndexBuffer(indexBufferSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &indexBuffer, nullptr) != D3D_OK)
			{
				indexBuffer = nullptr;
				indexBufferSize = 0;
			}
		}
	}

	// Update vertex and index buffers
	{
		CUSTOMVERTEX* vtx_dst;
		uint16_t* idx_dst;
		if (vertexBuffer->Lock(0, (UINT)(renderManager.totalVertexCount * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0)
		{
			state_block->Release();
			return;
		}
		if (indexBuffer->Lock(0, (UINT)(renderManager.totalIndexCount * sizeof(uint16_t)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0)
		{
			vertexBuffer->Unlock();
			state_block->Release();
			return;
		}

		for (size_t i = 0; i < renderManager.renderLists.size(); ++i)
		{
			ArRenderList* renderList = renderManager.renderLists[i];
			const ArVertex* vertices = renderList->vertices.data();
			for (size_t j = 0; j < renderList->vertices.size(); ++j)
			{
				const ArVertex& vtx = vertices[j];
				vtx_dst[j].pos[0] = vtx.position.x;
				vtx_dst[j].pos[1] = vtx.position.y;
				vtx_dst[j].pos[2] = 0.f;
				vtx_dst[j].col = COL_TO_DX9_ARGB(vtx.color);
				vtx_dst[j].uv[0] = vtx.uv.x;
				vtx_dst[j].uv[1] = vtx.uv.y;
			}
			const uint16_t* indices = renderList->indices.data();
			memcpy(idx_dst, indices, renderList->indices.size() * sizeof(uint16_t));

			renderList->vertices.free_data(); 
			renderList->indices.free_data();

			vtx_dst += renderList->vertices.size();
			idx_dst += renderList->indices.size();
		}
		indexBuffer->Unlock();
		vertexBuffer->Unlock();
		device->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
		device->SetIndices(indexBuffer);
		device->SetFVF(D3DFVF_CUSTOMVERTEX);
	}

	// Setup orthographic projection matrix
	{
		const float L = 0.5f;
		const float R = displayState.size.x + 0.5f;
		const float T = 0.5f;
		const float B = displayState.size.y + 0.5f;
		D3DMATRIX mat_identity = { { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } } };
		D3DMATRIX mat_projection =
		{ { {
			 2.0f / (R - L),		0.0f,					0.0f,			0.0f,
			 0.0f,					2.0f / (T - B),			0.0f,			0.0f,
			 0.0f,					0.0f,					0.5f,			0.0f,
			 (R + L) / (L - R),	(T + B) / (B - T),			0.5f,			1.0f,
		} } };
		device->SetTransform(D3DTS_WORLD, &mat_identity);
		device->SetTransform(D3DTS_VIEW, &mat_identity);
		device->SetTransform(D3DTS_PROJECTION, &mat_projection);
	}

	// Update font lands
	{
		for (auto& land : renderManager.lands)
		{
			if (!land.Ready() || !land.dirty)
				continue;

			IDirect3DTexture9* texture = (IDirect3DTexture9*)land.textureId;
			if (!texture)
				continue;
			if (!CopyTextureRegion(supportRGBA, texture, land.pixels, land.size.x, land.size.y, land.size.x))
			{
				land.textureId = nullptr;
				land.dirty = false;
				continue;
			}
			land.dirty = false;
		}
	}

	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = displayState.size.x;
	vp.Height = displayState.size.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	device->SetViewport(&vp);

	// Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing, shade mode (for gradient), bilinear sampling.
	device->SetPixelShader(nullptr);
	device->SetVertexShader(nullptr);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
	device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	device->SetRenderState(D3DRS_CLIPPING, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	size_t globalIndexOffset = 0;
	size_t globalVertexOffset = 0;
	for (auto& renderList : renderManager.renderLists)
	{
		for (auto& batch : renderList->batches)
		{
			const RECT clipRect = { (LONG)batch.scissor.min.x, (LONG)batch.scissor.min.y, (LONG)batch.scissor.max.x, (LONG)batch.scissor.max.y };
			device->SetScissorRect(&clipRect);

			device->SetTexture(0, (IDirect3DTexture9*)batch.texture);

			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (UINT)(globalVertexOffset), 0, (UINT)renderList->vertices.size(), (UINT)(globalIndexOffset + batch.indexOffset), (UINT)batch.indexCount / 3);
		}
		globalIndexOffset += renderList->indices.size();
		globalVertexOffset += renderList->vertices.size();
	}

	// Restore the DX9 transform
	device->SetTransform(D3DTS_WORLD, &last_world);
	device->SetTransform(D3DTS_VIEW, &last_view);
	device->SetTransform(D3DTS_PROJECTION, &last_projection);

	// Restore the DX9 state
	state_block->Apply();
	state_block->Release();
}

bool ArgonDx9Renderer::IsRunning() const
{
	if (!device)
		return false;
	return true;
}

bool ArgonDx9Renderer::CreateRendererObjects()
{
	{
		DX_SAFE_RELEASE(vertexBuffer);
		vertexBufferSize = 0x1400 * sizeof(ArVertex);
		if (device->CreateVertexBuffer(vertexBufferSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vertexBuffer, nullptr) != D3D_OK)
		{
			vertexBuffer = nullptr;
			vertexBufferSize = 0;
			return false;
		}
	}

	{
		DX_SAFE_RELEASE(indexBuffer);
		if (device->CreateIndexBuffer(indexBufferSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &indexBuffer, nullptr) != D3D_OK)
		{
			indexBuffer = nullptr;
			indexBufferSize = 0;
			return false;
		}
	}
	return true;
}

void ArgonDx9Renderer::ReleaseRendererObjects()
{
	DX_SAFE_RELEASE(vertexBuffer);

	DX_SAFE_RELEASE(indexBuffer);

	for (auto& texture : textures)
	{
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}
	}
}

IDirect3DTexture9* ArgonDx9Renderer::CreateTextureInternal(ArIntVec2 size, const void* pixels, DWORD usage)
{
	IDirect3DTexture9* texture = nullptr;
	if (device->CreateTexture(size.x, size.y, 1, usage, supportRGBA ? D3DFMT_A8B8G8R8 : D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) != D3D_OK)
		return nullptr;
	if (pixels)
	{
		if (!CopyTextureRegion(supportRGBA, texture, pixels, size.x, size.y, size.x))
		{
			texture->Release();
			return nullptr;
		}
	}
	return texture;
}
