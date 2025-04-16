#include "ArgonFreeTypeGlyphParser.h"
#include "../ArgonCore.h"
#include <freetype/ftmodapi.h>
#include <freetype/ftglyph.h>
#include <freetype/ftsynth.h>

#define FT_CEIL(x)  (((x) + 63) >> 6)

void* FT_ALLOC(FT_Memory memory, long size)
{
	return malloc(size);
}

void FT_FREE(FT_Memory memory, void* block)
{
	free(block);
}

void* FT_REALLOC(FT_Memory memory, long cur_size, long new_size, void* block)
{
	if (new_size == 0)
	{
		free(block);
		return nullptr;
	}
	if (cur_size == new_size)
		return block;
	void* newBlock = realloc(block, new_size);
	if (!newBlock)
		return nullptr;
	return newBlock;
}

void ArFreeTypeFontInfo::SetPixelHeight(uint32_t pixelHeight)
{
	FT_Size_RequestRec_ sizeRequest = {};
	sizeRequest.type = FT_SIZE_REQUEST_TYPE_NOMINAL;
	sizeRequest.width = 0;
	sizeRequest.height = pixelHeight * 64;
	sizeRequest.horiResolution = 0;
	sizeRequest.vertResolution = 0;
	FT_Error error = FT_Request_Size(ftFace, &sizeRequest);
	if (error != 0)
		return;

	FT_Size_Metrics metrics = ftFace->size->metrics;
	ascender = (float)FT_CEIL(metrics.ascender);
	descender = (float)FT_CEIL(metrics.descender);
	lineSpacing = (float)FT_CEIL(metrics.height);
	lineGap = (float)FT_CEIL(metrics.height - metrics.ascender + metrics.descender);
	maxAdvanceWidth = (float)FT_CEIL(metrics.max_advance);
}

bool ArgonFreeTypeGlyphParser::Awake()
{
	ftMemory = new FT_MemoryRec_;
	ftMemory->user = nullptr;
	ftMemory->alloc = &FT_ALLOC;
	ftMemory->free = &FT_FREE;
	ftMemory->realloc = &FT_REALLOC;
	FT_Error error = FT_New_Library(ftMemory, &ftLibrary);
	if (error != 0)
		return false;
	FT_Add_Default_Modules(ftLibrary);
	return true;
}

void ArgonFreeTypeGlyphParser::OnDestroy()
{
	for (auto& fontInfo : fontFaces)
	{
		if (fontInfo.ftFace)
		{
			FT_Done_Face(fontInfo.ftFace);
			fontInfo.ftFace = nullptr;
		}
	}
	if (ftLibrary)
	{
		FT_Done_Library(ftLibrary);
		ftLibrary = nullptr;
	}
	if (ftMemory)
	{
		delete ftMemory;
		ftMemory = nullptr;
	}
}

bool ArgonFreeTypeGlyphParser::HasGlyph(ArGuiID fontId, uint32_t codepoint) const
{
	if (fontFaces.size() <= fontId)
		return false;
	const ArFreeTypeFontInfo& fontInfo = fontFaces[fontId];
	const FT_Face& ftFace = fontInfo.ftFace;

	return FT_Get_Char_Index(ftFace, codepoint) != 0;
}

std::optional<ArgonFreeTypeGlyphParser::FontFaceInitResult> ArgonFreeTypeGlyphParser::InitFontFace(uint8_t* binary, size_t size)
{
	FT_Face ftFace = nullptr;
	FT_Error error = FT_New_Memory_Face(ftLibrary, (const FT_Byte*)binary, (FT_Long)size, 0, &ftFace);
	if (error != 0)
		return std::nullopt;
	error = FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE);
	if (error != 0)
	{
		FT_Done_Face(ftFace);
		return std::nullopt;
	}
	fontFaces.emplace_back(ftFace);
	FontFaceInitResult result = {};
	result.name = ftFace->family_name;
	result.fontId = (ArGuiID)(fontFaces.size() - 1);
	return result;
}

std::optional<IArgonGlyphParser::GlyphParseResult> ArgonFreeTypeGlyphParser::ParseGlyph(const ArFontFace& fontFace, const ArFontFace::GlyphCacheKey& key)
{
	if (key.codepoint <= 0 || key.size <= 0)
		return std::nullopt;
	ArFreeTypeFontInfo& fontInfo = fontFaces[fontFace.fontId];
	FT_Face& ftFace = fontInfo.ftFace;

	uint32_t glyphIndex = FT_Get_Char_Index(ftFace, key.codepoint);
	if (glyphIndex == 0)
		return std::nullopt;

	fontInfo.SetPixelHeight(key.size);

	FT_Error error = FT_Load_Glyph(ftFace, glyphIndex, 0);
	if (error != 0)
		return std::nullopt;

	FT_GlyphSlot& glyphSlot = ftFace->glyph;

	if (ArHasFlag(key.flags, ArGlyphFlag::Bold))
		FT_GlyphSlot_Embolden(glyphSlot);
	if (ArHasFlag(key.flags, ArGlyphFlag::Italic))
		FT_GlyphSlot_Oblique(glyphSlot);

	error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
	if (error != 0)
		return {};
	FT_Bitmap& bitmap = glyphSlot->bitmap;
	int width = bitmap.width;
	int height = bitmap.rows;
	int offsetX = glyphSlot->bitmap_left;
	int offsetY = glyphSlot->bitmap_top;

	IArgonGlyphParser::GlyphParseResult glyphParseResult = {};
	ArFontFace::GlyphInfo& glyphInfo = glyphParseResult.glyphInfo;
	glyphInfo.min = ArVec2((float)offsetX, fontInfo.ascender - (float)offsetY);
	glyphInfo.size = ArVec2((float)width, (float)height);

	glyphInfo.visible = glyphInfo.size.x > 0 && glyphInfo.size.y > 0;
	glyphInfo.advanceX = (float)FT_CEIL(glyphSlot->advance.x);
	glyphInfo.colored = bitmap.pixel_mode == FT_PIXEL_MODE_BGRA;

	glyphParseResult.pixels = new uint32_t[bitmap.width * bitmap.rows];
	uint32_t w = bitmap.width;
	uint32_t h = bitmap.rows;
	const uint8_t* src = bitmap.buffer;
	const uint32_t pitch = bitmap.pitch;
	uint32_t* dst = (uint32_t*)glyphParseResult.pixels;

	if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		for (uint32_t y = 0; y < h; y++, src += pitch, dst += w)
			for (uint32_t x = 0; x < w; x++)
				dst[x] = AR_COL32(255, 255, 255, src[x]);
	}
	else if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
	{
		uint8_t color0 = 0;
		uint8_t color1 = 255;

		for (uint32_t y = 0; y < h; y++, src += pitch, dst += w)
		{
			uint8_t bits = 0;
			const uint8_t* bits_ptr = src;
			for (uint32_t x = 0; x < w; x++, bits <<= 1)
			{
				if ((x & 7) == 0)
					bits = *bits_ptr++;
				dst[x] = AR_COL32(255, 255, 255, (bits & 0x80) ? color1 : color0);
			}
		}
	}
	return glyphParseResult;
}