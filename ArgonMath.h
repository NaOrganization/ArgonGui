#pragma once
#undef min
#undef max

#if (defined __SSE__ || defined __x86_64__ || defined _M_X64 || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1))) && !defined(IMGUI_DISABLE_SSE)
#define ARGUI_ENABLE_SSE
#include <immintrin.h>
#endif

// ---------------------------------------------------------- //
//  !.Argon basic data type
// ---------------------------------------------------------- //
class ArIntVec2 final
{
public:
	int x = 0;
	int y = 0;

	ArIntVec2() {}
	ArIntVec2(int x, int y) : x(x), y(y) {}
};

class ArVec2 final
{
public:
	float x = 0.f;
	float y = 0.f;

	ArVec2() {}
	ArVec2(const float& x, const float& y)
		: x(x), y(y) {
	}

	ArVec2 Normalized() const
	{
		float length = sqrtf(x * x + y * y);
		if (length > 0.f)
			return ArVec2(x / length, y / length);
		return ArVec2(0.f, 0.f);
	}

	float Length() const
	{
		return sqrtf(x * x + y * y);
	}
};

class ArVec3 final
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	ArVec3() {}
	ArVec3(const float& x, const float& y, const float& z)
		: x(x), y(y), z(z) {
	}
};

class ArVec4 final
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	ArVec4() {}
	ArVec4(const float& x, const float& y, const float& z, const float& w)
		: x(x), y(y), z(z), w(w) {
	}
};

// ---------------------------------------------------------- //
//  !.Argon math functions
// ---------------------------------------------------------- //
inline ArIntVec2 operator+(const ArIntVec2& lhs, const ArIntVec2& rhs) { return ArIntVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ArIntVec2 operator-(const ArIntVec2& lhs, const ArIntVec2& rhs) { return ArIntVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline ArIntVec2 operator*(const ArIntVec2& lhs, const ArIntVec2& rhs) { return ArIntVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline ArIntVec2 operator*(const ArIntVec2& lhs, int s) { return ArIntVec2(lhs.x * s, lhs.y * s); }
inline ArIntVec2 operator*(int s, const ArIntVec2& lhs) { return ArIntVec2(s * lhs.x, s * lhs.y); }
inline ArIntVec2 operator/(const ArIntVec2& lhs, const ArIntVec2& rhs) { return ArIntVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
inline ArIntVec2 operator/(const ArIntVec2& lhs, int s) { return ArIntVec2(lhs.x / s, lhs.y / s); }
inline ArIntVec2 operator-(const ArIntVec2& lhs) { return ArIntVec2(-lhs.x, -lhs.y); }
inline ArIntVec2& operator+=(ArIntVec2& lhs, const ArIntVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
inline ArIntVec2& operator-=(ArIntVec2& lhs, const ArIntVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
inline ArIntVec2& operator*=(ArIntVec2& lhs, const ArIntVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
inline ArIntVec2& operator*=(ArIntVec2& lhs, int s) { lhs.x *= s; lhs.y *= s; return lhs; }
inline ArIntVec2& operator/=(ArIntVec2& lhs, const ArIntVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
inline ArIntVec2& operator/=(ArIntVec2& lhs, int s) { lhs.x /= s; lhs.y /= s; return lhs; }

inline ArVec2 operator+(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ArVec2 operator-(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline ArVec2 operator*(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline ArVec2 operator/(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
inline bool operator==(const ArVec2& lhs, const ArVec2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline ArVec2 operator+(const ArVec2& lhs, float rhs) { return ArVec2(lhs.x + rhs, lhs.y + rhs); }
inline ArVec2 operator-(const ArVec2& lhs, float rhs) { return ArVec2(lhs.x - rhs, lhs.y - rhs); }
inline ArVec2 operator*(const ArVec2& lhs, float rhs) { return ArVec2(lhs.x * rhs, lhs.y * rhs); }
inline ArVec2 operator/(const ArVec2& lhs, float rhs) { return ArVec2(lhs.x / rhs, lhs.y / rhs); }
inline ArVec2 operator+(float lhs, const ArVec2& rhs) { return ArVec2(lhs + rhs.x, lhs + rhs.y); }
inline ArVec2 operator-(float lhs, const ArVec2& rhs) { return ArVec2(lhs - rhs.x, lhs - rhs.y); }
inline ArVec2 operator*(float lhs, const ArVec2& rhs) { return ArVec2(lhs * rhs.x, lhs * rhs.y); }
inline ArVec2 operator/(float lhs, const ArVec2& rhs) { return ArVec2(lhs / rhs.x, lhs / rhs.y); }
inline ArVec2 operator+=(ArVec2& lhs, const ArVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
inline ArVec2 operator-=(ArVec2& lhs, const ArVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
inline ArVec2 operator*=(ArVec2& lhs, const ArVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
inline ArVec2 operator*=(ArVec2& lhs, float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
inline ArVec2 operator/=(ArVec2& lhs, const ArVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
inline ArVec2 operator/=(ArVec2& lhs, float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }

inline ArVec3 operator+(const ArVec3& lhs, const ArVec3& rhs) { return ArVec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline ArVec3 operator-(const ArVec3& lhs, const ArVec3& rhs) { return ArVec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
inline ArVec3 operator*(const ArVec3& lhs, const ArVec3& rhs) { return ArVec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
inline ArVec3 operator/(const ArVec3& lhs, const ArVec3& rhs) { return ArVec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
inline ArVec3 operator+(const ArVec3& lhs, float rhs) { return ArVec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
inline ArVec3 operator-(const ArVec3& lhs, float rhs) { return ArVec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
inline ArVec3 operator*(const ArVec3& lhs, float rhs) { return ArVec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
inline ArVec3 operator/(const ArVec3& lhs, float rhs) { return ArVec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
inline ArVec3 operator+(float lhs, const ArVec3& rhs) { return ArVec3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }
inline ArVec3 operator-(float lhs, const ArVec3& rhs) { return ArVec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
inline ArVec3 operator*(float lhs, const ArVec3& rhs) { return ArVec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
inline ArVec3 operator/(float lhs, const ArVec3& rhs) { return ArVec3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
inline ArVec3 operator+=(ArVec3& lhs, const ArVec3& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }
inline ArVec3 operator-=(ArVec3& lhs, const ArVec3& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }
inline ArVec3 operator*=(ArVec3& lhs, const ArVec3& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z; return lhs; }
inline ArVec3 operator*=(ArVec3& lhs, float rhs) { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
inline ArVec3 operator/=(ArVec3& lhs, const ArVec3& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z; return lhs; }
inline ArVec3 operator/=(ArVec3& lhs, float rhs) { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }

inline ArVec4 operator+(const ArVec4& lhs, const ArVec4& rhs) { return ArVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline ArVec4 operator-(const ArVec4& lhs, const ArVec4& rhs) { return ArVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline ArVec4 operator*(const ArVec4& lhs, const ArVec4& rhs) { return ArVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
inline ArVec4 operator/(const ArVec4& lhs, const ArVec4& rhs) { return ArVec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }
inline ArVec4 operator+(const ArVec4& lhs, float rhs) { return ArVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
inline ArVec4 operator-(const ArVec4& lhs, float rhs) { return ArVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
inline ArVec4 operator*(const ArVec4& lhs, float rhs) { return ArVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline ArVec4 operator/(const ArVec4& lhs, float rhs) { return ArVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }
inline ArVec4 operator+(float lhs, const ArVec4& rhs) { return ArVec4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w); }
inline ArVec4 operator-(float lhs, const ArVec4& rhs) { return ArVec4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w); }
inline ArVec4 operator*(float lhs, const ArVec4& rhs) { return ArVec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
inline ArVec4 operator/(float lhs, const ArVec4& rhs) { return ArVec4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }
inline ArVec4 operator+=(ArVec4& lhs, const ArVec4& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; lhs.w += rhs.w; return lhs; }
inline ArVec4 operator-=(ArVec4& lhs, const ArVec4& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; lhs.w -= rhs.w; return lhs; }
inline ArVec4 operator*=(ArVec4& lhs, const ArVec4& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z; lhs.w *= rhs.w; return lhs; }
inline ArVec4 operator*=(ArVec4& lhs, float rhs) { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; lhs.w *= rhs; return lhs; }
inline ArVec4 operator/=(ArVec4& lhs, const ArVec4& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z; lhs.w /= rhs.w; return lhs; }
inline ArVec4 operator/=(ArVec4& lhs, float rhs) { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; lhs.w /= rhs; return lhs; }

#define AR_PI 3.14159265358979323846f
#define AR_ROUND(_VAL) ((float)(int)((_VAL) + 0.5f))
#define AR_DEG2RAD(_VAL) ((_VAL) * (AR_PI / 180.0f))

template<typename T> inline T ArMin(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> inline T ArMax(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }
template<typename T> inline T ArClamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> inline T ArLerp(T a, T b, float t) { return (T)(a + (b - a) * t); }
template<typename T> inline void ArSwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template<typename T> inline T ArAddClampOverflow(T a, T b, T mn, T mx) { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> inline T ArSubClampOverflow(T a, T b, T mn, T mx) { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }

inline ArVec2 ArMin(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
inline ArVec2 ArMax(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
inline ArVec2 ArClamp(const ArVec2& v, const ArVec2& mn, ArVec2 mx) { return ArVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
inline ArVec2 ArLerp(const ArVec2& a, const ArVec2& b, float t) { return ArVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
inline ArVec2 ArLerp(const ArVec2& a, const ArVec2& b, const ArVec2& t) { return ArVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
inline ArVec4 ArLerp(const ArVec4& a, const ArVec4& b, float t) { return ArVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
inline float  ArSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
inline float  ArLengthSqr(const ArVec2& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
inline float  ArLengthSqr(const ArVec4& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
inline float  ArFloor(float f) { return (float)(int)(f); }
inline float  ArFloorSigned(float f) { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent replacement for floorf()
inline ArVec2 ArFloor(const ArVec2& v) { return ArVec2((float)(int)(v.x), (float)(int)(v.y)); }
inline ArVec2 ArFloorSigned(const ArVec2& v) { return ArVec2(ArFloorSigned(v.x), ArFloorSigned(v.y)); }
inline int    ArModPositive(int a, int b) { return (a + b) % b; }
inline float  ArDot(const ArVec2& a, const ArVec2& b) { return a.x * b.x + a.y * b.y; }
inline ArVec2 ArRotate(const ArVec2& v, float cos_a, float sin_a) { return ArVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
inline float  ArLinearSweep(float current, float target, float speed) { if (current < target) return ArMin(current + speed, target); if (current > target) return ArMax(current - speed, target); return current; }
inline ArVec2 ArMul(const ArVec2& lhs, const ArVec2& rhs) { return ArVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline bool   ArIsFloatAboveGuaranteedIntegerPrecision(float f) { return f <= -16777216 || f >= 16777216; }
inline float  ArExponentialMovingAverage(float avg, float sample, int n) { avg -= avg / n; avg += sample / n; return avg; }

inline float  ArSign(float x) { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; }
inline double ArSign(double x) { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
#ifdef ARGUI_ENABLE_SSE
inline float  ArRsqrt(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
inline float  ArRsqrt(float x) { return 1.0f / sqrtf(x); }
#endif
inline double ArRsqrt(double x) { return 1.0 / sqrt(x); }

class ArRect
{
public:
	ArVec2 min = ArVec2();
	ArVec2 max = ArVec2();
	ArRect() {}
	ArRect(ArVec2 min, ArVec2 max) : min(min), max(max) {}
	ArRect(const ArRect& other) : min(other.min), max(other.max) {}
	ArRect(float x1, float y1, float x2, float y2) : min(x1, y1), max(x2, y2) {}


	ArVec2      GetCenter() const { return ArVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f); }
	ArVec2      GetSize() const { return ArVec2(max.x - min.x, max.y - min.y); }
	float       GetWidth() const { return max.x - min.x; }
	float       GetHeight() const { return max.y - min.y; }
	float       GetArea() const { return (max.x - min.x) * (max.y - min.y); }
	ArVec2      GetTL() const { return min; }                   // Top-left
	ArVec2      GetTR() const { return ArVec2(max.x, min.y); }  // Top-right
	ArVec2      GetBL() const { return ArVec2(min.x, max.y); }  // Bottom-left
	ArVec2      GetBR() const { return max; }                   // Bottom-right
	bool        Contains(const ArVec2& p) const { return p.x >= min.x && p.y >= min.y && p.x < max.x && p.y < max.y; }
	bool        Contains(const ArRect& r) const { return r.min.x >= min.x && r.min.y >= min.y && r.max.x <= max.x && r.max.y <= max.y; }
	bool        Overlaps(const ArRect& r) const { return r.min.y <  max.y && r.max.y >  min.y && r.min.x <  max.x && r.max.x >  min.x; }
	bool		Adjacent(const ArRect& r) const { return r.min.y == max.y || r.max.y == min.y || r.min.x == max.x || r.max.x == min.x; }
	ArRect		Union(const ArRect& r) const { return ArRect(ArMin(min, r.min), ArMax(max, r.max)); }
	ArRect		Intersection(const ArRect& r) const { return ArRect(ArMax(min, r.min), ArMin(max, r.max)); }
	void        Add(const ArVec2& p) { if (min.x > p.x)     min.x = p.x;     if (min.y > p.y)     min.y = p.y;     if (max.x < p.x)     max.x = p.x;     if (max.y < p.y)     max.y = p.y; }
	void        Add(const ArRect& r) { if (min.x > r.min.x) min.x = r.min.x; if (min.y > r.min.y) min.y = r.min.y; if (max.x < r.max.x) max.x = r.max.x; if (max.y < r.max.y) max.y = r.max.y; }
	void        Expand(const float amount) { min.x -= amount;   min.y -= amount;   max.x += amount;   max.y += amount; }
	void        Expand(const ArVec2& amount) { min.x -= amount.x; min.y -= amount.y; max.x += amount.x; max.y += amount.y; }
	void        Translate(const ArVec2& d) { min.x += d.x; min.y += d.y; max.x += d.x; max.y += d.y; }
	void        TranslateX(float dx) { min.x += dx; max.x += dx; }
	void        TranslateY(float dy) { min.y += dy; max.y += dy; }
	void        ClipWith(const ArRect& r) { min = ArMax(min, r.min); max = ArMin(max, r.max); }                   // Simple version, may lead to an inverted rectangle, which is fine for Contains/Overlaps test but not for display.
	void        ClipWithFull(const ArRect& r) { min = ArClamp(min, r.min, r.max); max = ArClamp(max, r.min, r.max); } // Full version, ensure both points are fully clipped.
	void        Floor() { min.x = (float)(int)(min.x); min.y = (float)(int)(min.y); max.x = (float)(int)(max.x); max.y = (float)(int)(max.y); }
	bool        IsInverted() const { return min.x > max.x || min.y > max.y; }
	ArVec4      ToVec4() const { return ArVec4(min.x, min.y, max.x, max.y); }
	void		AddOffset(const ArVec2& offset) { min.x += offset.x; min.y += offset.y; max.x += offset.x; max.y += offset.y; }
};

class ArMat3x3 final
{
public:
	float m[3][3] = { };

	ArMat3x3() {}
	ArMat3x3(float diagonal)
	{
		for (int i = 0; i < 3; ++i)
			m[i][i] = diagonal;
	}

	static ArMat3x3 Identity() { return ArMat3x3(1.f); }
	static ArMat3x3 Translation(const ArVec2& translation)
	{
		ArMat3x3 result = Identity();
		result.m[0][2] = translation.x;
		result.m[1][2] = translation.y;
		return result;
	}
	static ArMat3x3 Rotation(float angle)
	{
		ArMat3x3 result = Identity();
		angle = angle * AR_PI / 180.f;
		float cos_a = cosf(angle);
		float sin_a = sinf(angle);
		result.m[0][0] = cos_a;
		result.m[0][1] = -sin_a;
		result.m[1][0] = sin_a;
		result.m[1][1] = cos_a;
		return result;
	}
	static ArMat3x3 Scale(const ArVec2& scale)
	{
		ArMat3x3 result = Identity();
		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		return result;
	}

	float* operator[](int i) { return m[i]; }
	const float* operator[](int i) const { return m[i]; }

	ArMat3x3 operator*(const ArMat3x3& rhs) const
	{
		ArMat3x3 result;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				result.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j];
		return result;
	}

	ArVec2 operator*(const ArVec2& rhs) const
	{
		return ArVec2((m[0][0] * rhs.x) + (m[0][1] * rhs.y) + m[0][2], (m[1][0] * rhs.x) + (m[1][1] * rhs.y) + m[1][2]);
	}
	ArRect operator*(const ArRect& rhs) const
	{
		return ArRect((*this) * rhs.min, (*this) * rhs.max);
	}
};

class ArMat4x4 final
{
public:
	float m[4][4] = { };

	ArMat4x4() {}
	ArMat4x4(float diagonal)
	{
		for (int i = 0; i < 4; ++i)
			m[i][i] = diagonal;
	}

	static ArMat4x4 Identity() { return ArMat4x4(1.f); }

	float* operator[](int i) { return m[i]; }
	const float* operator[](int i) const { return m[i]; }
};

template <typename T, size_t BlockSize = 2042, typename Allocator = std::allocator<T>>
class ArChunkedVector {
private:
	Allocator allocator = {};
	std::vector<T*> chunks = {};
	T* currentChunk = nullptr;
	size_t currentIndex = 0;
	size_t elementSize = 0;
	T* contiguousData = nullptr;

	void allocate_block() {
		T* newBlock = allocator.allocate(BlockSize);
		chunks.push_back(newBlock);
		currentChunk = newBlock;
		currentIndex = 0;
	}

	void EnsureCapacityForOne() {
		if (currentChunk == nullptr || currentIndex >= BlockSize)
			allocate_block();
	}

public:
	ArChunkedVector() : currentChunk(nullptr), currentIndex(0), elementSize(0), contiguousData(nullptr) {}

	~ArChunkedVector() {
		clear();
		for (T* block : chunks)
			allocator.deallocate(block, BlockSize);
		chunks.clear();
		free_data();
	}

	ArChunkedVector(const ArChunkedVector&) = delete;
	ArChunkedVector& operator=(const ArChunkedVector&) = delete;

	void push_back(const T& value) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(value);
		++currentIndex;
		++elementSize;
	}

	void push_back(T&& value) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(std::move(value));
		++currentIndex;
		++elementSize;
	}

	template<typename... Args>
	void emplace_back(Args&&... args) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(std::forward<Args>(args)...);
		++currentIndex;
		++elementSize;
	}

	T* data() {
		if (contiguousData)
			return contiguousData;
		if (elementSize == 0)
			return nullptr;
		contiguousData = new T[elementSize];
		size_t copied = 0;
		for (size_t b = 0; b < chunks.size(); ++b) {
			size_t count = (b == chunks.size() - 1) ? currentIndex : BlockSize;
			std::copy(chunks[b], chunks[b] + count, contiguousData + copied);
			copied += count;
		}
		return contiguousData;
	}

	void free_data() {
		if (contiguousData) {
			delete[] contiguousData;
			contiguousData = nullptr;
		}
	}

	T& operator[](size_t index) {
		if (index >= elementSize)
			throw std::out_of_range("Index out of range");
		size_t blockIndex = index / BlockSize;
		size_t offset = index % BlockSize;
		return chunks[blockIndex][offset];
	}

	const T& operator[](size_t index) const {
		if (index >= elementSize)
			throw std::out_of_range("Index out of range");
		size_t blockIndex = index / BlockSize;
		size_t offset = index % BlockSize;
		return chunks[blockIndex][offset];
	}

	void clear() {
		for (size_t b = 0; b < chunks.size(); ++b) {
			size_t count = (b == chunks.size() - 1) ? currentIndex : BlockSize;
			for (size_t j = 0; j < count; ++j)
				(chunks[b] + j)->~T();
		}
		elementSize = 0;
		currentChunk = (chunks.empty() ? nullptr : chunks[0]);
		currentIndex = 0;
		free_data();
	}

	size_t size() const { return elementSize; }

	size_t capacity() const { return chunks.size() * BlockSize; }

	void reserve(size_t newCapacity) {
		size_t currentCapacity = capacity();
		while (currentCapacity < newCapacity) {
			allocate_block();
			currentCapacity += BlockSize;
		}
	}

	void resize(size_t newSize) {
		if (newSize < elementSize) {
			size_t newFullBlocks = newSize / BlockSize;
			size_t newLastCount = newSize % BlockSize;
			if (!chunks.empty() && newSize > 0) {
				if (newFullBlocks < chunks.size()) {
					size_t destroyCount = (newFullBlocks == chunks.size() - 1) ? (currentIndex - newLastCount) : BlockSize;
					T* block = chunks[newFullBlocks];
					for (size_t j = newLastCount; j < ((newFullBlocks == chunks.size() - 1) ? currentIndex : BlockSize); ++j)
						(block + j)->~T();
				}
			}
			elementSize = newSize;
			if (newSize == 0) {
				currentChunk = nullptr;
				currentIndex = 0;
			}
			else {
				size_t fullBlocks = newSize / BlockSize;
				size_t last = newSize % BlockSize;
				if (last == 0) {
					currentChunk = chunks[fullBlocks - 1];
					currentIndex = BlockSize;
				}
				else {
					currentChunk = chunks[fullBlocks];
					currentIndex = last;
				}
			}
		}
		else if (newSize > elementSize) {
			while (elementSize < newSize)
				push_back(T());
		}
	}
};

static const constexpr uint32_t ArCrc32LookupTable[256] =
{
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

template<typename T, typename = void>
struct ArIsEnumFlag : std::false_type {};

template<typename T>
struct ArIsEnumFlag<T, std::enable_if_t<
	std::is_enum<T>::value&&
	std::is_same<typename std::underlying_type<T>::type, uint32_t>::value&&
	std::is_convertible<decltype(T::None), T>::value
	>> : std::true_type {};

// 使用 enable_if 替代 requires
template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, Enum>::type
operator|(Enum lhs, Enum rhs)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, Enum>::type
operator&(Enum lhs, Enum rhs)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, bool>::type
ArHasFlag(Enum value, Enum flag)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return (static_cast<Underlying>(value) & static_cast<Underlying>(flag)) != 0;
}

template<typename Enum, typename... EnumFlags>
typename std::enable_if<ArIsEnumFlag<Enum>::value && (ArIsEnumFlag<EnumFlags>::value && ...), bool>::type
ArHasAnyFlag(Enum value, EnumFlags... flags)
{
	return (ArHasFlag(value, flags) || ...);
}