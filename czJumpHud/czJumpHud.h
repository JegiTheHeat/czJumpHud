#pragma once
#include <sstream>
#include <chrono>
#include <time.h>

static bool czJumpHudMenu;
struct hudElement
{
	bool enabled;
	float opacity = 0.5f;
	float fontSize = 0.65f;
	ImVec4 tColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
};

class hud
{
public:
	static inline hudElement* vel = new hudElement;
	static inline  hudElement* pos = new hudElement;

};

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_x[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

struct Vec2
{
public:
	Vec2() { x = 0;  y = 0; };
	Vec2(float vec_x, float vec_y) { x = vec_x; y = vec_y; }
	Vec2(const Vec2& v) { x = v.x; y = v.y; }
	inline Vec2 operator + (const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	inline Vec2 operator - (const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	inline Vec2 operator / (const Vec2& v) const { return Vec2(x / v.x, y / v.y); }
	inline Vec2 operator * (const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
	inline bool operator == (const Vec2& v) const { return x == v.x && y == v.y; }
	inline bool operator != (const Vec2& v) const { return x != v.x && y != v.y; }
	inline Vec2& operator += (const Vec2& v) { this->x += v.x; this->y += v.y; }
	inline Vec2& operator -= (const Vec2& v) { this->x += v.x; this->y += v.y; }
	inline Vec2& operator *= (const Vec2& v) { this->x += v.x; this->y += v.y; }
	inline Vec2& operator /= (const Vec2& v) { this->x += v.x; this->y += v.y; }
	inline float& operator [] (int index) { float f = FLT_MIN; if (index == 0) return x;  if (index == 1) return y; return f; }
	const float& operator [] (int index) const { if (index == 0) return x;  if (index == 1) return y; return FLT_MIN; }
	ImVec2 ToImVec2() const { return ImVec2(x, y); }
	float x, y;
};
struct Vec3
{
	float x, y, z;
	Vec3() { x = 0; y = 0; z = 0; };
	Vec3(vec3_x vec) { x = vec[0]; y = vec[1]; z = vec[2]; }
	Vec3(float vec_x, float vec_y, float vec_z) { x = vec_x; y = vec_y; z = vec_z; }
	Vec3(const Vec3& v) { x = v.x; y = v.y; z = v.z; }
	inline Vec3 operator + (const Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
	inline Vec3 operator - (const Vec3& v) { return Vec3(x - v.x, y - v.y, z - v.z); }
	inline Vec3 operator / (const Vec3& v) { return Vec3(x / v.x, y / v.y, z / v.z); }
	inline Vec3 operator * (const Vec3& v) { return Vec3(x * v.x, y * v.y, z * v.z); }
	inline bool operator == (const Vec3& v) { return x == v.x && y == v.y && z == v.z; }
	inline bool operator != (const Vec3& v) { return x != v.x || y != v.y || z != v.z; }
	inline float& operator [] (int index) { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; return z; }
	const float& operator [] (int index) const { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; return FLT_MIN; }
	inline Vec3& operator += (const Vec3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
	inline Vec3& operator -= (const Vec3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
	inline Vec3& operator *= (const Vec3& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; return *this; }
	inline Vec3& operator /= (const Vec3& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; return *this; }
	inline float Length() { return sqrt(x * x + y * y + z * z); }
	inline float Length2D() { return sqrt(x * x + y * y); }
	inline float Dist(const Vec3 v) { return (*this - v).Length(); }
	//inline vec3_t* v3_t() { vec3_t rval = { x, y, z }; return &rval; }
	Vec2 const ScreenLocation();
	Vec2 const Angles();
};


typedef struct {
	int				x;
	int				y;
	int				Width;
	int				Height;
	float           Fovx;
	float           Fovy;
	Vec3			Vieworg;
	Vec3			Viewaxis[3];
	char            unknown142[0x4050];
	Vec3			refdefViewAngles;
} refdef_t;


class CViewAngles
{
public:
	float y;
	float x;
};

