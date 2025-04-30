#pragma once
#include "Misc/stb_rect_pack.hpp"
#include <optional>
#include <future>

// ----------------------------------------------------------- //
//  [ARGON] [ArgonRender] Enum class declarations
// ----------------------------------------------------------- //

// [FLAGS] [ArgonRenderSystem] - Flags for rendering.
enum class ArRenderFlag : uint32_t
{
	Closed = 1 << 0,
	None = 0
};

// [FLAGS] [ArgonRenderSystem] - Flags for rendering list.
enum class ArRenderListFlag : uint32_t
{
	UseAntiAliasing = 1 << 0,
	None = 0
};

// [FLAGS] [ArgonRenderSystem] - Flags for glyphs.
enum class ArGlyphFlag : uint32_t
{
	Bold = 1 << 0,
	Italic = 1 << 1,
	None = 0
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonRender] Interface declarations
// ----------------------------------------------------------- //

// [INTERFACE] [ArgonRenderSystem] - Glyph parser initialization interface. This interface is used to initialize font face from font files.
class IArGlyphParserConfig
{
public:
	virtual ~IArGlyphParserConfig() {}
};

// [INTERFACE] [ArgonRenderSystem] - Glyph parser interface. This interface is used to parse glyphs from font files.
class IArgonGlyphParser
{
public:
	class GlyphParseResult final
	{
	public:
		ArGlyphInfo* glyphInfo = nullptr;
		uint32_t* pixels = nullptr;
	};
	class FontFaceInitResult final
	{
	public:
		std::string name = {};
		ArGuiID fontId = 0;
	};

	virtual ~IArgonGlyphParser() {}

	virtual bool Awake(const IArGlyphParserConfig& config) = 0;

	virtual void OnDestroy() = 0;

	virtual bool HasGlyph(ArGuiID fontId, uint32_t codepoint) const = 0;

	virtual std::optional<FontFaceInitResult> InitFontFace(uint8_t* binary, size_t size) = 0;

	virtual std::optional<GlyphParseResult> ParseGlyph(const ArFont& font, const ArGlyphKey& key) = 0;
};

// [INTERFACE] [ArgonRenderSystem] - Renderer initialization interface. This interface is used to create renderer.
class IArRendererConfig
{
public:
	virtual ~IArRendererConfig() {}
};

// [INTERFACE] [ArgonRenderSystem] - Renderer interface. This interface is used to create renderer.
class IArRenderCustomCreateConfig
{
public:
	virtual ~IArRenderCustomCreateConfig() {}
};

// [INTERFACE] [ArgonRenderSystem] - Renderer interface. This interface is used to create renderer.
class IArgonRenderer
{
public:
	virtual ArString Name() const = 0;

	virtual bool Awake(const IArRendererConfig& config) = 0;

	virtual void StartFrame(const ArgonRenderManager& renderManager) = 0;

	virtual void EndFrame(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) = 0;

	virtual void OnDestroy() = 0;

	virtual ArTextureID CreateTexture(ArIntVec2 size, const void* pixels) = 0;

	virtual ArShaderID CreateCustomShader(const IArRenderCustomCreateConfig& config) = 0;

	virtual void SetCurrentVertexShader(ArShaderID shader) = 0;

	virtual bool SetVertexShaderConstantBuffer(const void* data, size_t size) = 0;

	virtual void SetCurrentPixelShader(ArShaderID shader) = 0;

	virtual bool SetPixelShaderConstantBuffer(const void* data, size_t size) = 0;

	virtual void ReleaseTexture(ArTextureID texture) = 0;

	virtual void ReleaseCustomShader(ArShaderID shader) = 0;

	virtual void Present(const ArgonRenderManager& renderManager, const ArDisplayState& displayState) = 0;

	virtual bool IsRunning() const = 0;
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonRender] Base rendering pipeline struct
// ----------------------------------------------------------- //

// [STRUCT] [ArgonRenderSystem] - Vertex structure.
class ArVertex final
{
public:
	ArVec2 position = ArVec2();
	ArVec2 uv = ArVec2();
	uint32_t color = 0x0;

	ArVertex() {}
	ArVertex(const ArVec2& position, ArVec2 uv, uint32_t color)
		: position(position), uv(uv), color(color) {
	}
};

// [STRUCT] [ArgonRenderSystem] - Render batch structure.
class ArRenderBatch final
{
public:
	size_t indexCount = 0;
	size_t indexOffset = 0;
	ArTextureID texture = nullptr;
	ArRect scissor = ArRect();

	ArRenderBatch() {}
	ArRenderBatch(size_t indexCount, size_t indexOffset, ArTextureID texture, ArRect scissor)
		: indexCount(indexCount), indexOffset(indexOffset), texture(texture), scissor(scissor) {
	}
};

// [STRUCT] [ArgonRenderSystem] - Shared data for render list. Because we don't use DI(Dependency Injection) and it's bad to pass render manager to every render list.
class ArRenderListSharedData final
{
public:
	ArVec2 uvOfWhitInTexture = ArVec2();
	std::array<ArVec4, 64> uvOfBakeLines = {};
	ArgonRenderManager* renderManager = nullptr;
	std::array<ArVec2, 48> arcFastVtx = {};
	float arcFastRadiusCutoff = 0.f;
	float circleSegmentMaxError = 0.3f;
	std::array<uint8_t, 64> circleSegmentCounts = {};

	ArRenderListSharedData();
};

// [CLASS] [ArgonRenderSystem] - Render list class. This class is used to store all the render commands and then render them in one go.
class ArRenderList
{
protected:
	ArRenderBatch* currentBatch = nullptr;
	ArTextureID currentTexture = nullptr;
	ArShaderID currentCustomPixelShader = nullptr;
	ArShaderID currentCustomVertexShader = nullptr;
	ArRenderListSharedData* sharedData = nullptr;
	ArRect currentScissor = ArRect();

	std::vector<ArVec2> path = {};

	std::vector<ArTextureID> textureStack = {};
	std::vector<ArRect> scissorStack = {};
	std::vector<ArShaderID> customPixelShaderStack = {};
	std::vector<ArShaderID> customVertexShaderStack = {};
public:
	std::vector<ArRenderBatch> batches = {};
	ArChunkedVector<ArVertex> vertices = {};
	ArChunkedVector<uint16_t> indices = {};

	ArRenderListFlag listFlags = ArRenderListFlag::UseAntiAliasing;

	ArRenderList(ArRenderListSharedData* sharedData);
	~ArRenderList() { Clear(); }

	void AddLine(const ArVec2& from, const ArVec2& to, uint32_t color, float thickness = 1.f);

	void AddRect(const ArRect& rect, uint32_t color, float thickness = 1.f, ArVec4 rounding = {}, ArRenderFlag flags = ArRenderFlag::None);

	void AddRectFilled(const ArRect& rect, uint32_t color, ArVec4 rounding = {}, ArRenderFlag flags = ArRenderFlag::None);

	void AddRectFilledMultiColor(const ArRect& rect, uint32_t colorUpperLeft, uint32_t colorUpperRight, uint32_t colorBottomRight, uint32_t colorBottomLeft);

	void AddQuad(const ArVec2& a, const ArVec2& b, const ArVec2& c, const ArVec2& d, uint32_t color, float thickness = 1.f);

	void AddQuadFilled(const ArVec2& a, const ArVec2& b, const ArVec2& c, const ArVec2& d, uint32_t color);

	void AddTriangle(const ArVec2& a, const ArVec2& b, const ArVec2& c, uint32_t color);

	void AddTriangleFilled(const ArVec2& a, const ArVec2& b, const ArVec2& c, uint32_t color);

	void AddCircle(const ArVec2& center, float radius, uint32_t color, float thickness = 1.f, int numSegments = 0);

	void AddCircleFilled(const ArVec2& center, float radius, uint32_t color, int numSegments = 0);

	void AddImage(ArTextureID texture, const ArRect& rect, const ArRect& uv, uint32_t color, ArRenderFlag flags = ArRenderFlag::None);

	void AddPolyline(const std::vector<ArVec2>& points, uint32_t color, ArRenderFlag flags = ArRenderFlag::None, float thickness = 1.f);

	void AddConvexPolyFilled(const std::vector<ArVec2>& points, uint32_t color);

	void AddText(ArStringView text, uint32_t size, const ArVec2& pos, uint32_t color);


	void PushTexture(ArTextureID texture);

	void PushScissor(ArRect scissor);

	void PopTexture();

	void PopScissor();

	ArTextureID GetCurrentTexture() const { return currentTexture; }

	void Clear();
protected:
	void PrimRectWithUV(const ArRect& rect, const ArRect& uv, uint32_t color);

	void PrimRectWithCornerColor(const ArRect& rect, const ArVec2& uv, uint32_t colorUpperLeft, uint32_t colorUpperRight, uint32_t colorBottomRight, uint32_t colorBottomLeft);

	void PathClear() { path.resize(0); }

	virtual void PathLineTo(const ArVec2& pos) { path.push_back(pos); }

	void PathRect(const ArRect& rect, ArVec4 rounding);

	void PathArcTo(const ArVec2& center, float radius, float aMin, float aMax, int numSegments = 0);

	void PathArcToN(const ArVec2& center, float radius, float aMin, float aMax, int numSegments);

	void PathArcToFast(const ArVec2& center, float radius, int aMinSample, int aMaxSample, int aStep);

	void PathArcToFast(const ArVec2& center, float radius, int aMinOf12, int aMaxOf12);

	void PathFillConvex(uint32_t color) { AddConvexPolyFilled(path, color); PathClear(); }

	void PathStroke(uint32_t color, ArRenderFlag flags = ArRenderFlag::None, float thickness = 1.f) { AddPolyline(path, color, flags, thickness); PathClear(); }


	void AddNewBatch();

	void RequireSpace(size_t vertexCount, size_t indexCount);

	void OnChangedTexture();

	void OnChangedScissor();

	int CalcCircleAutoSegmentCount(float radius) const;
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonRender] Texture management
// ----------------------------------------------------------- //

// [CLASS] [ArgonRenderSystem] - Texture land class. This class is used to store all the textures in one land.
class ArTextureLand final
{
public:
	const ArIntVec2 size = ArIntVec2(4096, 4096);
	const ArVec2 uvScale = ArVec2(1.f / size.x, 1.f / size.y);

	stbrp_context rectPackerContext = {};
	std::vector<stbrp_node> nodes = std::vector<stbrp_node>(size.x);

	uint32_t* pixels = new uint32_t[size.x * size.y]();
	mutable ArTextureID textureId = nullptr;
	mutable bool dirty = false;
public:
	ArTextureLand() { stbrp_init_target(&rectPackerContext, size.x, size.y, nodes.data(), size.x); }
	~ArTextureLand() {}

	void OnDestroy(IArgonRenderer* renderer);

	bool Ready() const { return textureId != nullptr; }

	ArTextureID GetTexture() const { if (textureId != nullptr) return textureId; return nullptr; }
};

// [STRUCT] [ArgonRenderSystem] - Glyph key structure. This structure is used to store the key for a glyph.
class ArGlyphKey final
{
public:
	uint32_t codepoint = 0;
	uint32_t size = 0;
	ArGlyphFlag flags = ArGlyphFlag::None;
public:
	bool operator==(const ArGlyphKey& other) const
	{
		return codepoint == other.codepoint && size == other.size && flags == other.flags;
	}
	struct Hash final
	{
		size_t operator()(const ArGlyphKey& key) const
		{
			return std::hash<uint32_t>()(key.codepoint) ^ std::hash<uint32_t>()(key.size) ^ std::hash<uint32_t>()(static_cast<uint32_t>(key.flags));
		}
	};

	ArGlyphKey() {}
	ArGlyphKey(const uint32_t& codepoint, const uint32_t& size, const ArGlyphFlag& flags)
		: codepoint(codepoint), size(size), flags(flags) {
	}
};

// [STRUCT] [ArgonRenderSystem] - Glyph info structure. This structure is used to store the info for a glyph.
class ArGlyphInfo final
{
public:
	uint32_t textureAtlasIndex = 0;
	bool visible = false;
	bool colored = false;
	ArVec2 min = ArVec2(0.f, 0.f);
	ArVec2 size = ArVec2(0.f, 0.f);
	ArRect uv = ArRect();
	float advanceX = 0.f;
};

// [CLASS] [ArgonRenderSystem] - Font face class. This class is used to store all the glyphs for a font face.
class ArFont final
{
public:
	using GlyphCacheKey = ArGlyphKey;
	class GlyphMappingTask final
	{
	public:
		GlyphCacheKey key = GlyphCacheKey();
		uint32_t* pixels = nullptr;

		GlyphMappingTask(const GlyphCacheKey& key, uint32_t* pixels)
			: key(key), pixels(pixels) {
		}
	};
	
	IArgonGlyphParser* glyphParser = nullptr;

	const ArGuiID fontId = 0;
	const ArString name = {};

	std::queue<GlyphMappingTask> mappingQueue = {};

	std::unordered_map<GlyphCacheKey, ArGlyphInfo, GlyphCacheKey::Hash> glyphs = {};
	ArGlyphInfo* fallbackGlyph = nullptr;
public:
	ArFont(IArgonGlyphParser* glyphParser, ArGuiID fontId, ArStringView name)
		: glyphParser(glyphParser), fontId(fontId), name(name) { }
	ArFont& operator=(const ArFont&) = delete;
	~ArFont() {}

	void Awake();

	void EndFrame(ArgonRenderManager& renderManager);

	ArGlyphInfo* GetGlyphNoFallback(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);

	ArGlyphInfo* TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);
private:
	ArGlyphInfo* FindGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);
};

// ----------------------------------------------------------- //
//  [ARGON] [ArgonRender] ArgonRenderManager declaration
// ----------------------------------------------------------- //

// [CLASS] [ArgonRenderSystem] - Render manager class. This class is used to manage all the render lists and textures.
class ArgonRenderManager final
{
public:
	class Territory final
	{
	public:
		uint32_t landIndex = 0;
		ArIntVec2 size = ArIntVec2();
		ArIntVec2 position = ArIntVec2();

		ArRect GetRect() const { return ArRect(position, position + size); }
	};
	class NewFontRequest final
	{
	public:
		void* fontBinary = nullptr;
		size_t binarySize = 0;

		NewFontRequest(void* binary, size_t size) : fontBinary(binary), binarySize(size) {}
	};
public:
	std::vector<ArTextureLand> lands = {};
	std::vector<Territory> territories = {};
	mutable std::vector<ArFont> fonts = {};

	std::queue<NewFontRequest> newFontRequests = {};

	ArRenderListSharedData sharedData = {};
	std::vector<ArRenderList*> renderLists = {};
	size_t totalVertexCount = 0;
	size_t totalIndexCount = 0;
public:
	ArgonRenderManager() {}
	ArgonRenderManager(const ArgonRenderManager&) = delete;
	ArgonRenderManager& operator=(const ArgonRenderManager&) = delete;
	~ArgonRenderManager() {}

	void Awake();

	void StartFrame(IArgonGlyphParser* glyphParser);

	void EndFrame();

	void PostPresent();

	void OnDestroy(IArgonRenderer* renderer);

	template<typename T, typename... Args>
	T* AllocationRenderList(Args&&... args) { T* renderList = new T(&sharedData, std::forward<Args>(args)...); renderLists.push_back(renderList); return renderList; }

	void AddFontFromCompressed(void* data, size_t size);

	void AddFontFromCompressedBase85(const uint8_t* base85);

	void AddFontFromFile(const std::filesystem::path& path);

	ArTextureID GetDefaultTexture() const;

	ArGlyphInfo* TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;

	std::vector<ArGlyphInfo*> TryGetGlyphs(ArStringView text, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;

	ArVec2 CalcTextSize(ArStringView text, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;

	Territory AllocateTerritory(int width, int height);
private:
	void BuildBaseTerritories(ArRenderListSharedData& sharedData);
};
