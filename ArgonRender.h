#pragma once
#include "Misc/stb_rect_pack.hpp"
#include <optional>
#include <future>

#define AR_COL32_R_SHIFT    0
#define AR_COL32_G_SHIFT    8
#define AR_COL32_B_SHIFT    16
#define AR_COL32_A_SHIFT    24
#define AR_COL32_A_MASK     0xFF000000

#define AR_COL32(R,G,B,A)    (((uint32_t)(A)<<AR_COL32_A_SHIFT) | ((uint32_t)(B)<<AR_COL32_B_SHIFT) | ((uint32_t)(G)<<AR_COL32_G_SHIFT) | ((uint32_t)(R)<<AR_COL32_R_SHIFT))
#define AR_COL32_WHITE       AR_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define AR_COL32_BLACK       AR_COL32(0,0,0,255)        // Opaque black
#define AR_COL32_BLACK_TRANS AR_COL32(0,0,0,0)          // Transparent black = 0x00000000

// ---------------------------------------------------------- //
//  !. Enum class declarations
// ---------------------------------------------------------- //

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
//  !. Rendering basic structures
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
	~ArRenderList() {}

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
//  !. About Texture Atlas
// ----------------------------------------------------------- //

// [CLASS] [ArgonRenderSystem] - Texture atlas class. This class is used to store all the textures in one atlas.
class ArTextureAtlas final
{
public:
	const ArIntVec2 atlasSize = ArIntVec2(4096, 4096);
	const ArVec2 uvScale = ArVec2(1.f / atlasSize.x, 1.f / atlasSize.y);

	stbrp_context rectPackerContext = {};
	std::vector<stbrp_node> nodes = {};

	uint32_t* pixels = nullptr; // use rgba32 format.
	mutable ArTextureID textureId = nullptr;
	mutable bool dirty = false;
public:
	ArTextureAtlas() : nodes(atlasSize.x), pixels(new uint32_t[atlasSize.x * atlasSize.y]()) { stbrp_init_target(&rectPackerContext, atlasSize.x, atlasSize.y, nodes.data(), atlasSize.x); }
	~ArTextureAtlas() {}

	void OnDestroy(IArgonRenderer* renderer);

	bool Ready() const { return textureId != nullptr; }

	ArTextureID GetTexture() const { if (textureId != nullptr) return textureId; return nullptr; }
};

// [CLASS] [ArgonRenderSystem] - Font face class. This class is used to store all the glyphs for a font face.
class ArFontFace final
{
public:
	class GlyphCacheKey final
	{
	public:
		uint32_t codepoint = 0;
		uint32_t size = 0;
		ArGlyphFlag flags = ArGlyphFlag::None;
	public:
		bool operator==(const GlyphCacheKey& other) const
		{
			return codepoint == other.codepoint && size == other.size && flags == other.flags;
		}
		struct Hash final
		{
			size_t operator()(const GlyphCacheKey& key) const
			{
				return std::hash<uint32_t>()(key.codepoint) ^ std::hash<uint32_t>()(key.size) ^ std::hash<uint32_t>()(static_cast<uint32_t>(key.flags));
			}
		};

		GlyphCacheKey() {}
		GlyphCacheKey(const uint32_t& codepoint, const uint32_t& size, const ArGlyphFlag& flags)
			: codepoint(codepoint), size(size), flags(flags) {
		}
	};
	class GlyphInfo final
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
	class GlyphMappingTask final
	{
	public:
		GlyphCacheKey key = GlyphCacheKey();
		uint32_t* pixels = nullptr;

		GlyphMappingTask(const GlyphCacheKey& key, uint32_t* pixels)
			: key(key), pixels(pixels) {
		}
	};

	ArTextureManager& textureManager;

	const ArGuiID fontId = 0;
	const std::string name = {};

	std::queue<GlyphMappingTask> mappingQueue = {};

	std::unordered_map<GlyphCacheKey, GlyphInfo, GlyphCacheKey::Hash> glyphs = {};
	GlyphInfo* fallbackGlyph = nullptr;
public:
	ArFontFace(ArTextureManager& textureManager, ArGuiID fontId, const std::string& name)
		: textureManager(textureManager), fontId(fontId), name(name) {
	}
	ArFontFace& operator=(const ArFontFace&) = delete;
	~ArFontFace() {}

	void Awake();

	void EndFrame();

	GlyphInfo* GetGlyphNoFallback(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);

	GlyphInfo* TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);
private:
	GlyphInfo* FindGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None);
};

// [CLASS] [ArgonRenderSystem] - Texture manager class. This class is used to manage all the textures and font faces.
class ArTextureManager final
{
public:
	class Territory final
	{
	public:
		int atlasIndex = -1;
		ArIntVec2 size = ArIntVec2();
		ArIntVec2 position = ArIntVec2(-1, -1);

		ArRect GetRect() const { return ArRect((float)position.x, (float)position.y, (float)position.x + (float)size.x, (float)position.y + (float)size.y); }

		ArRect GetUV(ArIntVec2 atlasSize) const { const ArRect& rect = GetRect(); return ArRect(rect.min.x / atlasSize.x, rect.min.y / atlasSize.y, rect.max.x / atlasSize.x, rect.max.y / atlasSize.y); }

		bool IsValid() const { return position.x >= 0.f && position.y >= 0.f; }
	};
	class FontFaceQuery final
	{
	public:
		void* fontBinary = nullptr;
		size_t binarySize = 0;

		FontFaceQuery(void* binary, size_t size) : fontBinary(binary), binarySize(size) {}
	};
public:
	IArgonGlyphParser* glyphParser = nullptr;
	std::vector<ArTextureAtlas> atlases = {}; // normally only one atlas, except for very large glyphs
	mutable std::vector<ArFontFace> fontFaces = {};
	std::vector<Territory> territories = {};
	std::queue<FontFaceQuery> fontFaceQueryQueue = {};

	ArGuiID antiAliasingGlyphTerritoryId = 0;
	ArGuiID whitePixelTerritoryId = 0;
	bool buildedBaseTerritories = false;
public:
	ArTextureManager() {}
	ArTextureManager(const ArTextureManager&) = delete;
	ArTextureManager& operator=(const ArTextureManager&) = delete;
	~ArTextureManager() {}

	void Awake(ArRenderListSharedData& sharedData);

	void StartFrame();

	void EndFrame(ArRenderListSharedData& sharedData);

	Territory AllocateTerritory(int width, int height);

	ArFontFace::GlyphInfo* TryGetGlyph(uint32_t codepoint, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;

	std::vector<ArFontFace::GlyphInfo*> TryGetGlyphs(ArStringView text, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;

	ArVec2 CalcTextSize(ArStringView text, uint32_t size, ArGlyphFlag flags = ArGlyphFlag::None) const;
private:
	void BuildBaseTerritories(ArRenderListSharedData& sharedData);
};

// ------------------------------------------------------------ //
//  !. ArgonRenderManager declaration
// ------------------------------------------------------------ //

// [CLASS] [ArgonRenderSystem] - Render manager class. This class is used to manage all the render lists and textures.
class ArgonRenderManager final
{
public:
	ArTextureManager textureManager = {};
	ArRenderListSharedData sharedData = {};
	std::vector<ArRenderList*> renderLists = {};
	size_t totalVertexCount = 0;
	size_t totalIndexCount = 0;
public:
	ArgonRenderManager() {}
	ArgonRenderManager(const ArgonRenderManager&) = delete;
	ArgonRenderManager& operator=(const ArgonRenderManager&) = delete;
	~ArgonRenderManager() {}

	template<typename T, typename... Args>
	T* AllocationRenderList(Args&&... args)
	{
		T* renderList = new T(&sharedData, std::forward<Args>(args)...);
		renderLists.push_back(renderList);
		return renderList;
	}

	void AddFontFromCompressed(void* data, size_t size);

	void AddFontFromCompressedBase85(const uint8_t* base85);

	void AddFontFromFile(const std::filesystem::path& path);

	void Awake();

	void StartFrame();

	void EndFrame();

	void PostPresent();

	void OnDestroy(IArgonRenderer* renderer);

	ArTextureID GetDefaultTexture() const;
};

// ------------------------------------------------------------- //
//  !. ArgonRenderSystem interface declarations
// ------------------------------------------------------------- //

// [INTERFACE] [ArgonRenderSystem] - Glyph parser interface. This interface is used to parse glyphs from font files.
class IArgonGlyphParser
{
public:
	class GlyphParseResult final
	{
	public:
		ArFontFace::GlyphInfo glyphInfo = ArFontFace::GlyphInfo();
		uint32_t* pixels = nullptr;
	};
	class FontFaceInitResult final
	{
	public:
		std::string name = {};
		ArGuiID fontId = 0;
	};

	virtual ~IArgonGlyphParser() {}

	virtual bool Awake() = 0;

	virtual void OnDestroy() = 0;

	virtual bool HasGlyph(ArGuiID fontId, uint32_t codepoint) const = 0;

	virtual std::optional<FontFaceInitResult> InitFontFace(uint8_t* binary, size_t size) = 0;

	virtual std::optional<GlyphParseResult> ParseGlyph(const ArFontFace& fontFace, const ArFontFace::GlyphCacheKey& key) = 0;
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
	virtual std::string Name() const = 0;

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
