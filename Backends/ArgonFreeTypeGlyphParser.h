#pragma once
#include "../ArgonGui.h"
#include <freetype/freetype.h>

class ArFreeTypeFontInfo final
{
public:
	FT_Face ftFace = {};
	float ascender = 0.f;
	float descender = 0.f;
	float lineSpacing = 0.f;
	float lineGap = 0.f;
	float maxAdvanceWidth = 0.f;

	ArFreeTypeFontInfo(FT_Face ftFace) : ftFace(ftFace) {}
	void SetPixelHeight(uint32_t pixelHeight);
};

class ArgonFreeTypeGlyphParser final : public IArgonGlyphParser
{
public:
	FT_Library ftLibrary = {};
	FT_Memory ftMemory = nullptr;
	std::vector<ArFreeTypeFontInfo> fonts = {};
public:
	bool Awake(const IArGlyphParserConfig& config) override;

	void OnDestroy() override;

	bool HasGlyph(ArGuiID fontId, uint32_t codepoint) const override;

	std::optional<FontFaceInitResult> InitFontFace(uint8_t* binary, size_t size) override;

	std::optional<GlyphParseResult> ParseGlyph(const ArFont& font, const ArGlyphKey& key) override;
};