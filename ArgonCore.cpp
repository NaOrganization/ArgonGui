#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "ArgonCore.h"
#include <locale>
#include <codecvt>
#include <random>

ArTimePoint ArHelp::GetTimePoint()
{
	return std::chrono::time_point_cast<ArDuration>(std::chrono::system_clock::now());
}

ArGuiID ArHelp::Hash(ArStringView string, ArGuiID seed)
{
	seed = ~seed;
	uint32_t crc = seed;
	const unsigned char* data = (const unsigned char*)string.data();
	size_t data_size = string.size();
	const uint32_t* crc32_lut = ArCrc32LookupTable;
	if (data_size != 0)
	{
		while (data_size-- != 0)
		{
			unsigned char c = *data++;
			if (c == '#' && data_size >= 2 && data[0] == '#' && data[1] == '#')
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
		}
	}
	else
	{
		while (unsigned char c = *data++)
		{
			if (c == '#' && data[0] == '#' && data[1] == '#')
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
		}
	}
	return ~crc;
}

std::vector<uint32_t> ArHelp::Utf8::DecodeToCodepoints(ArStringView str)
{
	std::vector<uint32_t> result;
	const char* s = str.data();
	const char* end = str.data() + str.size();

	while (s < end) {
		uint8_t first = static_cast<uint8_t>(*s);
		uint8_t len = 0;
		uint32_t codepoint = 0;

		if (first < 0x80) {
			len = 1;
			codepoint = first;
		}
		else if ((first >> 5) == 0x6) {
			if (s + 1 >= end) break;
			len = 2;
			codepoint = ((first & 0x1F) << 6) | (static_cast<uint8_t>(s[1]) & 0x3F);
		}
		else if ((first >> 4) == 0xE) {
			if (s + 2 >= end) break;
			len = 3;
			codepoint = ((first & 0x0F) << 12)
				| ((static_cast<uint8_t>(s[1]) & 0x3F) << 6)
				| (static_cast<uint8_t>(s[2]) & 0x3F);
		}
		else if ((first >> 3) == 0x1E) {
			if (s + 3 >= end) break;
			len = 4;
			codepoint = ((first & 0x07) << 18)
				| ((static_cast<uint8_t>(s[1]) & 0x3F) << 12)
				| ((static_cast<uint8_t>(s[2]) & 0x3F) << 6)
				| (static_cast<uint8_t>(s[3]) & 0x3F);
		}
		else {
			break;
		}

		result.push_back(codepoint);
		s += len;
	}

	return result;
}

float ArHelp::Random::GetFloat(float min, float max)
{
	if (min > max)
		std::swap(min, max);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

