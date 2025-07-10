#pragma once

#define NOMINMAX
#include <Windows.h>

#if __has_include("DirectXMath.h")
// In this case, DirectXMath is coming from Windows SDK.
//	It is better to use this on Windows as some Windows libraries could depend on the same 
//	DirectXMath headers
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#endif

#define XM_CONSTEXPR

using namespace DirectX;
using namespace DirectX::PackedVector;


struct FVector2;
struct FVector4;
struct FMatrix;
struct FQuaternion;
struct FPlane;

// radian <-> degree
inline float Radian(const float degree)
{
    return degree * XM_PI / 180.0f;
}

inline float Degree(const float radian)
{
    return radian * 180.0f / XM_PI;
}

//template<class T> inline T XMMin(T a, T b) noexcept { return (a < b) ? a : b; }
//template<class T> inline T XMMax(T a, T b) noexcept { return (a > b) ? a : b; }

//------------------------------------------------------------------------------
// 2D rectangle
struct FRectangle
{
    long x;
    long y;
    long width;
    long height;

    // Creators
    FRectangle() noexcept : x(0), y(0), width(0), height(0) {}
    constexpr FRectangle(long ix, long iy, long iw, long ih) noexcept : x(ix), y(iy), width(iw), height(ih) {}
    explicit FRectangle(const RECT& rct) noexcept : x(rct.left), y(rct.top), width(rct.right - rct.left), height(rct.bottom - rct.top) {}

    FRectangle(const FRectangle&) = default;
    FRectangle& operator=(const FRectangle&) = default;

    FRectangle(FRectangle&&) = default;
    FRectangle& operator=(FRectangle&&) = default;

    operator RECT() noexcept { RECT rct; rct.left = x; rct.top = y; rct.right = (x + width); rct.bottom = (y + height); return rct; }
#ifdef __cplusplus_winrt
    operator Windows::Foundation::Rect() noexcept { return Windows::Foundation::Rect(float(x), float(y), float(width), float(height)); }
#endif

    // Comparison operators
#if (__cplusplus >= 202002L)
    bool operator == (const FRectangle&) const = default;
    auto operator <=> (const FRectangle&) const = default;
#else
    bool operator == (const FRectangle& r) const noexcept { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
    bool operator != (const FRectangle& r) const noexcept { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }
#endif
    bool operator == (const RECT& rct) const noexcept { return (x == rct.left) && (y == rct.top) && (width == (rct.right - rct.left)) && (height == (rct.bottom - rct.top)); }
    bool operator != (const RECT& rct) const noexcept { return (x != rct.left) || (y != rct.top) || (width != (rct.right - rct.left)) || (height != (rct.bottom - rct.top)); }

    // Assignment operators
    FRectangle& operator=(_In_ const RECT& rct) noexcept { x = rct.left; y = rct.top; width = (rct.right - rct.left); height = (rct.bottom - rct.top); return *this; }

    // FRectangle operations
    FVector2 Location() const noexcept;
    FVector2 Center() const noexcept;

    bool IsEmpty() const noexcept { return (width == 0 && height == 0 && x == 0 && y == 0); }

    bool Contains(long ix, long iy) const noexcept { return (x <= ix) && (ix < (x + width)) && (y <= iy) && (iy < (y + height)); }
    bool Contains(const FVector2& point) const noexcept;
    bool Contains(const FRectangle& r) const noexcept { return (x <= r.x) && ((r.x + r.width) <= (x + width)) && (y <= r.y) && ((r.y + r.height) <= (y + height)); }
    bool Contains(const RECT& rct) const noexcept { return (x <= rct.left) && (rct.right <= (x + width)) && (y <= rct.top) && (rct.bottom <= (y + height)); }

    void Inflate(long horizAmount, long vertAmount) noexcept;

    bool Intersects(const FRectangle& r) const noexcept { return (r.x < (x + width)) && (x < (r.x + r.width)) && (r.y < (y + height)) && (y < (r.y + r.height)); }
    bool Intersects(const RECT& rct) const noexcept { return (rct.left < (x + width)) && (x < rct.right) && (rct.top < (y + height)) && (y < rct.bottom); }

    void Offset(long ox, long oy) noexcept { x += ox; y += oy; }

    // Static functions
    static FRectangle Intersect(const FRectangle& ra, const FRectangle& rb) noexcept;
    static RECT Intersect(const RECT& rcta, const RECT& rctb) noexcept;

    static FRectangle Union(const FRectangle& ra, const FRectangle& rb) noexcept;
    static RECT Union(const RECT& rcta, const RECT& rctb) noexcept;
};

//------------------------------------------------------------------------------
// 2D vector
struct FVector2 : public XMFLOAT2
{
    FVector2() noexcept : XMFLOAT2(0.f, 0.f) {}
    constexpr explicit FVector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
    constexpr FVector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
    explicit FVector2(_In_reads_(2) const float* pArray) noexcept : XMFLOAT2(pArray) {}
    FVector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
    FVector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
    explicit FVector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

    FVector2(const FVector2&) = default;
    FVector2& operator=(const FVector2&) = default;

    FVector2(FVector2&&) = default;
    FVector2& operator=(FVector2&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

    // Comparison operators
    bool operator == (const FVector2& V) const noexcept;
    bool operator != (const FVector2& V) const noexcept;

    // Assignment operators
    FVector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
    FVector2& operator+= (const FVector2& V) noexcept;
    FVector2& operator-= (const FVector2& V) noexcept;
    FVector2& operator*= (const FVector2& V) noexcept;
    FVector2& operator*= (float S) noexcept;
    FVector2& operator/= (float S) noexcept;

    // Unary operators
    FVector2 operator+ () const noexcept { return *this; }
    FVector2 operator- () const noexcept { return FVector2(-x, -y); }

    // Vector operations
    bool InBounds(const FVector2& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector2& V) const noexcept;
    void Cross(const FVector2& V, FVector2& result) const noexcept;
    FVector2 Cross(const FVector2& V) const noexcept;

    void Normalize() noexcept;
    void Normalize(FVector2& result) const noexcept;

    void Clamp(const FVector2& vmin, const FVector2& vmax) noexcept;
    void Clamp(const FVector2& vmin, const FVector2& vmax, FVector2& result) const noexcept;

    // Static functions
    static float Distance(const FVector2& v1, const FVector2& v2) noexcept;
    static float DistanceSquared(const FVector2& v1, const FVector2& v2) noexcept;

    static void Min(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept;
    static FVector2 Min(const FVector2& v1, const FVector2& v2) noexcept;

    static void Max(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept;
    static FVector2 Max(const FVector2& v1, const FVector2& v2) noexcept;

    static void Lerp(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept;
    static FVector2 Lerp(const FVector2& v1, const FVector2& v2, float t) noexcept;

    static void SmoothStep(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept;
    static FVector2 SmoothStep(const FVector2& v1, const FVector2& v2, float t) noexcept;

    static void Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g, FVector2& result) noexcept;
    static FVector2 Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t, FVector2& result) noexcept;
    static FVector2 CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t) noexcept;

    static void Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t, FVector2& result) noexcept;
    static FVector2 Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t) noexcept;

    static void Reflect(const FVector2& ivec, const FVector2& nvec, FVector2& result) noexcept;
    static FVector2 Reflect(const FVector2& ivec, const FVector2& nvec) noexcept;

    static void Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex, FVector2& result) noexcept;
    static FVector2 Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex) noexcept;

    static void JTransform(const FVector2& v, const FQuaternion& quat, FVector2& result) noexcept;
    static FVector2 JTransform(const FVector2& v, const FQuaternion& quat) noexcept;

    static void JTransform(const FVector2& v, const FMatrix& m, FVector2& result) noexcept;
    static FVector2 JTransform(const FVector2& v, const FMatrix& m) noexcept;
    static void JTransform(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector2* resultArray) noexcept;

    static void JTransform(const FVector2& v, const FMatrix& m, FVector4& result) noexcept;
    static void JTransform(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    static void TransformNormal(const FVector2& v, const FMatrix& m, FVector2& result) noexcept;
    static FVector2 TransformNormal(const FVector2& v, const FMatrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector2* resultArray) noexcept;

    // Constants
    static const FVector2 Zero;
    static const FVector2 One;
    static const FVector2 UnitX;
    static const FVector2 UnitY;
};

// Binary operators
FVector2 operator+ (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator- (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator* (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator* (const FVector2& V, float S) noexcept;
FVector2 operator/ (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator/ (const FVector2& V, float S) noexcept;
FVector2 operator* (float S, const FVector2& V) noexcept;

//------------------------------------------------------------------------------
// 3D vector
struct FVector3 : public XMFLOAT3
{
    FVector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
    constexpr explicit FVector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
    constexpr FVector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
    explicit FVector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
    FVector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
    FVector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
    explicit FVector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

    FVector3(const FVector3&) = default;
    FVector3& operator=(const FVector3&) = default;

    FVector3(FVector3&&) = default;
    FVector3& operator=(FVector3&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

    // Comparison operators
    bool operator == (const FVector3& V) const noexcept;
    bool operator != (const FVector3& V) const noexcept;

    // Assignment operators
    FVector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
    FVector3& operator+= (const FVector3& V) noexcept;
    FVector3& operator-= (const FVector3& V) noexcept;
    FVector3& operator*= (const FVector3& V) noexcept;
    FVector3& operator*= (float S) noexcept;
    FVector3& operator/= (float S) noexcept;

    // Unary operators
    FVector3 operator+ () const noexcept { return *this; }
    FVector3 operator- () const noexcept;

    // Vector operations
    bool InBounds(const FVector3& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector3& V) const noexcept;
    void Cross(const FVector3& V, FVector3& result) const noexcept;
    FVector3 Cross(const FVector3& V) const noexcept;

    void Normalize() noexcept;
    void Normalize(FVector3& result) const noexcept;

    void Clamp(const FVector3& vmin, const FVector3& vmax) noexcept;
    void Clamp(const FVector3& vmin, const FVector3& vmax, FVector3& result) const noexcept;

    // Static functions
    static float Distance(const FVector3& v1, const FVector3& v2) noexcept;
    static float DistanceSquared(const FVector3& v1, const FVector3& v2) noexcept;

    static void Min(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept;
    static FVector3 Min(const FVector3& v1, const FVector3& v2) noexcept;

    static void Max(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept;
    static FVector3 Max(const FVector3& v1, const FVector3& v2) noexcept;

    static void Lerp(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept;
    static FVector3 Lerp(const FVector3& v1, const FVector3& v2, float t) noexcept;

    static void SmoothStep(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept;
    static FVector3 SmoothStep(const FVector3& v1, const FVector3& v2, float t) noexcept;

    static void Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g, FVector3& result) noexcept;
    static FVector3 Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t, FVector3& result) noexcept;
    static FVector3 CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t) noexcept;

    static void Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t, FVector3& result) noexcept;
    static FVector3 Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t) noexcept;

    static void Reflect(const FVector3& ivec, const FVector3& nvec, FVector3& result) noexcept;
    static FVector3 Reflect(const FVector3& ivec, const FVector3& nvec) noexcept;

    static void Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex, FVector3& result) noexcept;
    static FVector3 Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex) noexcept;

    static void JTransform(const FVector3& v, const FQuaternion& quat, FVector3& result) noexcept;
    static FVector3 JTransform(const FVector3& v, const FQuaternion& quat) noexcept;

    static void JTransform(const FVector3& v, const FMatrix& m, FVector3& result) noexcept;
    static FVector3 JTransform(const FVector3& v, const FMatrix& m) noexcept;
    static void JTransform(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector3* resultArray) noexcept;

    static void JTransform(const FVector3& v, const FMatrix& m, FVector4& result) noexcept;
    static void JTransform(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    static void TransformNormal(const FVector3& v, const FMatrix& m, FVector3& result) noexcept;
    static FVector3 TransformNormal(const FVector3& v, const FMatrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector3* resultArray) noexcept;

    // Constants
    static const FVector3 Zero;
    static const FVector3 One;
    static const FVector3 UnitX;
    static const FVector3 UnitY;
    static const FVector3 UnitZ;
    static const FVector3 Up;
    static const FVector3 Down;
    static const FVector3 Right;
    static const FVector3 Left;
    static const FVector3 Forward;
    static const FVector3 Backward;
};

// Binary operators
FVector3 operator+ (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator- (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator* (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator* (const FVector3& V, float S) noexcept;
FVector3 operator/ (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator/ (const FVector3& V, float S) noexcept;
FVector3 operator* (float S, const FVector3& V) noexcept;

//------------------------------------------------------------------------------
// 4D vector
struct FVector4 : public XMFLOAT4
{
    FVector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
    constexpr explicit FVector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
    constexpr FVector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    explicit FVector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FVector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FVector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
    explicit FVector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    FVector4(const FVector4&) = default;
    FVector4& operator=(const FVector4&) = default;

    FVector4(FVector4&&) = default;
    FVector4& operator=(FVector4&&) = default;

    operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const FVector4& V) const noexcept;
    bool operator != (const FVector4& V) const noexcept;

    // Assignment operators
    FVector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
    FVector4& operator+= (const FVector4& V) noexcept;
    FVector4& operator-= (const FVector4& V) noexcept;
    FVector4& operator*= (const FVector4& V) noexcept;
    FVector4& operator*= (float S) noexcept;
    FVector4& operator/= (float S) noexcept;

    // Unary operators
    FVector4 operator+ () const noexcept { return *this; }
    FVector4 operator- () const noexcept;

    // Vector operations
    bool InBounds(const FVector4& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector4& V) const noexcept;
    void Cross(const FVector4& v1, const FVector4& v2, FVector4& result) const noexcept;
    FVector4 Cross(const FVector4& v1, const FVector4& v2) const noexcept;

    void Normalize() noexcept;
    void Normalize(FVector4& result) const noexcept;

    void Clamp(const FVector4& vmin, const FVector4& vmax) noexcept;
    void Clamp(const FVector4& vmin, const FVector4& vmax, FVector4& result) const noexcept;

    // Static functions
    static float Distance(const FVector4& v1, const FVector4& v2) noexcept;
    static float DistanceSquared(const FVector4& v1, const FVector4& v2) noexcept;

    static void Min(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept;
    static FVector4 Min(const FVector4& v1, const FVector4& v2) noexcept;

    static void Max(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept;
    static FVector4 Max(const FVector4& v1, const FVector4& v2) noexcept;

    static void Lerp(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept;
    static FVector4 Lerp(const FVector4& v1, const FVector4& v2, float t) noexcept;

    static void SmoothStep(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept;
    static FVector4 SmoothStep(const FVector4& v1, const FVector4& v2, float t) noexcept;

    static void Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g, FVector4& result) noexcept;
    static FVector4 Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t, FVector4& result) noexcept;
    static FVector4 CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t) noexcept;

    static void Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t, FVector4& result) noexcept;
    static FVector4 Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t) noexcept;

    static void Reflect(const FVector4& ivec, const FVector4& nvec, FVector4& result) noexcept;
    static FVector4 Reflect(const FVector4& ivec, const FVector4& nvec) noexcept;

    static void Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex, FVector4& result) noexcept;
    static FVector4 Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex) noexcept;

    static void JTransform(const FVector2& v, const FQuaternion& quat, FVector4& result) noexcept;
    static FVector4 JTransform(const FVector2& v, const FQuaternion& quat) noexcept;

    static void JTransform(const FVector3& v, const FQuaternion& quat, FVector4& result) noexcept;
    static FVector4 JTransform(const FVector3& v, const FQuaternion& quat) noexcept;

    static void JTransform(const FVector4& v, const FQuaternion& quat, FVector4& result) noexcept;
    static FVector4 JTransform(const FVector4& v, const FQuaternion& quat) noexcept;

    static void JTransform(const FVector4& v, const FMatrix& m, FVector4& result) noexcept;
    static FVector4 JTransform(const FVector4& v, const FMatrix& m) noexcept;
    static void JTransform(_In_reads_(count) const FVector4* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    // Constants
    static const FVector4 Zero;
    static const FVector4 One;
    static const FVector4 UnitX;
    static const FVector4 UnitY;
    static const FVector4 UnitZ;
    static const FVector4 UnitW;
};

// Binary operators
FVector4 operator+ (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator- (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator* (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator* (const FVector4& V, float S) noexcept;
FVector4 operator/ (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator/ (const FVector4& V, float S) noexcept;
FVector4 operator* (float S, const FVector4& V) noexcept;

//------------------------------------------------------------------------------
// 4x4 FMatrix (assumes right-handed cooordinates)
struct FMatrix : public XMFLOAT4X4
{
    FMatrix() noexcept
        : XMFLOAT4X4(1.f, 0, 0, 0,
            0, 1.f, 0, 0,
            0, 0, 1.f, 0,
            0, 0, 0, 1.f)
    {
    }
    constexpr FMatrix(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) noexcept
        : XMFLOAT4X4(m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33)
    {
    }
    explicit FMatrix(const FVector3& r0, const FVector3& r1, const FVector3& r2) noexcept
        : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
            r1.x, r1.y, r1.z, 0,
            r2.x, r2.y, r2.z, 0,
            0, 0, 0, 1.f)
    {
    }
    explicit FMatrix(const FVector4& r0, const FVector4& r1, const FVector4& r2, const FVector4& r3) noexcept
        : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
            r1.x, r1.y, r1.z, r1.w,
            r2.x, r2.y, r2.z, r2.w,
            r3.x, r3.y, r3.z, r3.w)
    {
    }
    FMatrix(const XMFLOAT4X4& M) noexcept { memcpy(this, &M, sizeof(XMFLOAT4X4)); }
    FMatrix(const XMFLOAT3X3& M) noexcept;
    FMatrix(const XMFLOAT4X3& M) noexcept;

    explicit FMatrix(_In_reads_(16) const float* pArray) noexcept : XMFLOAT4X4(pArray) {}
    FMatrix(CXMMATRIX M) noexcept { XMStoreFloat4x4(this, M); }

    FMatrix(const FMatrix&) = default;
    FMatrix& operator=(const FMatrix&) = default;

    FMatrix(FMatrix&&) = default;
    FMatrix& operator=(FMatrix&&) = default;

    operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

    // Comparison operators
    bool operator == (const FMatrix& M) const noexcept;
    bool operator != (const FMatrix& M) const noexcept;

    // Assignment operators
    FMatrix& operator= (const XMFLOAT3X3& M) noexcept;
    FMatrix& operator= (const XMFLOAT4X3& M) noexcept;
    FMatrix& operator+= (const FMatrix& M) noexcept;
    FMatrix& operator-= (const FMatrix& M) noexcept;
    FMatrix& operator*= (const FMatrix& M) noexcept;
    FMatrix& operator*= (float S) noexcept;
    FMatrix& operator/= (float S) noexcept;

    FMatrix& operator/= (const FMatrix& M) noexcept;
    // Element-wise divide

// Unary operators
    FMatrix operator+ () const noexcept { return *this; }
    FMatrix operator- () const noexcept;

    // Properties
    FVector3 Up() const noexcept { return FVector3(_21, _22, _23); }
    void Up(const FVector3& v) noexcept { _21 = v.x; _22 = v.y; _23 = v.z; }

    FVector3 Down() const  noexcept { return FVector3(-_21, -_22, -_23); }
    void Down(const FVector3& v) noexcept { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

    FVector3 Right() const noexcept { return FVector3(_11, _12, _13); }
    void Right(const FVector3& v) noexcept { _11 = v.x; _12 = v.y; _13 = v.z; }

    FVector3 Left() const noexcept { return FVector3(-_11, -_12, -_13); }
    void Left(const FVector3& v) noexcept { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

    FVector3 Forward() const noexcept { return FVector3(-_31, -_32, -_33); }
    void Forward(const FVector3& v) noexcept { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

    FVector3 Backward() const noexcept { return FVector3(_31, _32, _33); }
    void Backward(const FVector3& v) noexcept { _31 = v.x; _32 = v.y; _33 = v.z; }

    FVector3 Translation() const  noexcept { return FVector3(_41, _42, _43); }
    void Translation(const FVector3& v) noexcept { _41 = v.x; _42 = v.y; _43 = v.z; }

    // FMatrix operations
    bool Decompose(FVector3& scale, FQuaternion& rotation, FVector3& translation) noexcept;

    FMatrix Transpose() const noexcept;
    void Transpose(FMatrix& result) const noexcept;

    FMatrix Invert() const noexcept;
    void Invert(FMatrix& result) const noexcept;

    float Determinant() const noexcept;

    // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
    FVector3 ToEuler() const noexcept;

    // Static functions
    static FMatrix CreateBillboard(
        const FVector3& object, const FVector3& cameraPosition, const FVector3& cameraUp, _In_opt_ const FVector3* cameraForward = nullptr) noexcept;

    static FMatrix CreateConstrainedBillboard(
        const FVector3& object, const FVector3& cameraPosition, const FVector3& rotateAxis,
        _In_opt_ const FVector3* cameraForward = nullptr, _In_opt_ const FVector3* objectForward = nullptr) noexcept;

    static FMatrix CreateTranslation(const FVector3& position) noexcept;
    static FMatrix CreateTranslation(float x, float y, float z) noexcept;

    static FMatrix CreateScale(const FVector3& scales) noexcept;
    static FMatrix CreateScale(float xs, float ys, float zs) noexcept;
    static FMatrix CreateScale(float scale) noexcept;

    static FMatrix CreateRotationX(float radians) noexcept;
    static FMatrix CreateRotationY(float radians) noexcept;
    static FMatrix CreateRotationZ(float radians) noexcept;

    static FMatrix CreateFromAxisAngle(const FVector3& axis, float angle) noexcept;

    static FMatrix CreatePerspectiveFieldOfViewRH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspectiveRH(float width, float height, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspectiveFieldOfViewLH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspectiveLH(float width, float height, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
    static FMatrix CreateOrthographicRH(float width, float height, float zNearPlane, float zFarPlane) noexcept;
    static FMatrix CreateOrthographicOffCenterRH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
    static FMatrix CreateOrthographicLH(float width, float height, float zNearPlane, float zFarPlane) noexcept;
    static FMatrix CreateOrthographicOffCenterLH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

    static FMatrix CreateLookAtRH(const FVector3& position, const FVector3& target, const FVector3& up) noexcept;
    static FMatrix CreateLookAtLH(const FVector3& position, const FVector3& target, const FVector3& up) noexcept;
    static FMatrix CreateLookToRH(const FVector3& position, const FVector3& target, const FVector3& up) noexcept;
    static FMatrix CreateLookToLH(const FVector3& position, const FVector3& target, const FVector3& up) noexcept;

    static FMatrix CreateWorld(const FVector3& position, const FVector3& forward, const FVector3& up) noexcept;

    static FMatrix CreateFromQuaternion(const FQuaternion& quat) noexcept;

    // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
    static FMatrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

    // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
    static FMatrix CreateFromYawPitchRoll(const FVector3& angles) noexcept;

    static FMatrix CreateShadow(const FVector3& lightDir, const FPlane& plane) noexcept;

    static FMatrix CreateReflection(const FPlane& plane) noexcept;

    static void Lerp(const FMatrix& M1, const FMatrix& M2, float t, FMatrix& result) noexcept;
    static FMatrix Lerp(const FMatrix& M1, const FMatrix& M2, float t) noexcept;

    static void JTransform(const FMatrix& M, const FQuaternion& rotation, FMatrix& result) noexcept;
    static FMatrix JTransform(const FMatrix& M, const FQuaternion& rotation) noexcept;

    // Constants
    static const FMatrix Identity;
};

// Binary operators
FMatrix operator+ (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator- (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator* (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator* (const FMatrix& M, float S) noexcept;
FMatrix operator/ (const FMatrix& M, float S) noexcept;
FMatrix operator/ (const FMatrix& M1, const FMatrix& M2) noexcept;
// Element-wise divide
FMatrix operator* (float S, const FMatrix& M) noexcept;


//-----------------------------------------------------------------------------
// FPlane
struct FPlane : public XMFLOAT4
{
    FPlane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
    constexpr FPlane(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    FPlane(const FVector3& normal, float d) noexcept : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
    FPlane(const FVector3& point1, const FVector3& point2, const FVector3& point3) noexcept;
    FPlane(const FVector3& point, const FVector3& normal) noexcept;
    explicit FPlane(const FVector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    explicit FPlane(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FPlane(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FPlane(const XMFLOAT4& p) noexcept { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
    explicit FPlane(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    FPlane(const FPlane&) = default;
    FPlane& operator=(const FPlane&) = default;

    FPlane(FPlane&&) = default;
    FPlane& operator=(FPlane&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const FPlane& p) const noexcept;
    bool operator != (const FPlane& p) const noexcept;

    // Assignment operators
    FPlane& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

    // Properties
    FVector3 Normal() const noexcept { return FVector3(x, y, z); }
    void Normal(const FVector3& normal) noexcept { x = normal.x; y = normal.y; z = normal.z; }

    float D() const noexcept { return w; }
    void D(float d) noexcept { w = d; }

    // FPlane operations
    void Normalize() noexcept;
    void Normalize(FPlane& result) const noexcept;

    float Dot(const FVector4& v) const noexcept;
    float DotCoordinate(const FVector3& position) const noexcept;
    float DotNormal(const FVector3& normal) const noexcept;

    // Static functions
    static void JTransform(const FPlane& plane, const FMatrix& M, FPlane& result) noexcept;
    static FPlane JTransform(const FPlane& plane, const FMatrix& M) noexcept;

    static void JTransform(const FPlane& plane, const FQuaternion& rotation, FPlane& result) noexcept;
    static FPlane JTransform(const FPlane& plane, const FQuaternion& rotation) noexcept;
    // Input quaternion must be the inverse transpose of the transformation
};

//------------------------------------------------------------------------------
// FQuaternion
struct FQuaternion : public XMFLOAT4
{
    FQuaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
    constexpr FQuaternion(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    FQuaternion(const FVector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
    explicit FQuaternion(const FVector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    explicit FQuaternion(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FQuaternion(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FQuaternion(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
    explicit FQuaternion(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    FQuaternion(const FQuaternion&) = default;
    FQuaternion& operator=(const FQuaternion&) = default;

    FQuaternion(FQuaternion&&) = default;
    FQuaternion& operator=(FQuaternion&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const FQuaternion& q) const noexcept;
    bool operator != (const FQuaternion& q) const noexcept;

    // Assignment operators
    FQuaternion& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
    FQuaternion& operator+= (const FQuaternion& q) noexcept;
    FQuaternion& operator-= (const FQuaternion& q) noexcept;
    FQuaternion& operator*= (const FQuaternion& q) noexcept;
    FQuaternion& operator*= (float S) noexcept;
    FQuaternion& operator/= (const FQuaternion& q) noexcept;

    // Unary operators
    FQuaternion operator+ () const  noexcept { return *this; }
    FQuaternion operator- () const noexcept;

    // FQuaternion operations
    float Length() const noexcept;
    float LengthSquared() const noexcept;

    void Normalize() noexcept;
    void Normalize(FQuaternion& result) const noexcept;

    void Conjugate() noexcept;
    void Conjugate(FQuaternion& result) const noexcept;

    void Inverse(FQuaternion& result) const noexcept;

    float Dot(const FQuaternion& Q) const noexcept;

    void RotateTowards(const FQuaternion& target, float maxAngle) noexcept;
    void __cdecl RotateTowards(const FQuaternion& target, float maxAngle, FQuaternion& result) const noexcept;

    // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
    FVector3 ToEuler() const noexcept;

    // Static functions
    static FQuaternion CreateFromAxisAngle(const FVector3& axis, float angle) noexcept;

    // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
    static FQuaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

    // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
    static FQuaternion CreateFromYawPitchRoll(const FVector3& angles) noexcept;

    static FQuaternion CreateFromRotationMatrix(const FMatrix& M) noexcept;

    static void Lerp(const FQuaternion& q1, const FQuaternion& q2, float t, FQuaternion& result) noexcept;
    static FQuaternion Lerp(const FQuaternion& q1, const FQuaternion& q2, float t) noexcept;

    static void Slerp(const FQuaternion& q1, const FQuaternion& q2, float t, FQuaternion& result) noexcept;
    static FQuaternion Slerp(const FQuaternion& q1, const FQuaternion& q2, float t) noexcept;

    static void Concatenate(const FQuaternion& q1, const FQuaternion& q2, FQuaternion& result) noexcept;
    static FQuaternion Concatenate(const FQuaternion& q1, const FQuaternion& q2) noexcept;

    static void __cdecl FromToRotation(const FVector3& fromDir, const FVector3& toDir, FQuaternion& result) noexcept;
    static FQuaternion FromToRotation(const FVector3& fromDir, const FVector3& toDir) noexcept;

    static void __cdecl LookRotation(const FVector3& forward, const FVector3& up, FQuaternion& result) noexcept;
    static FQuaternion LookRotation(const FVector3& forward, const FVector3& up) noexcept;

    static float Angle(const FQuaternion& q1, const FQuaternion& q2) noexcept;

    // Constants
    static const FQuaternion Identity;
};

// Binary operators
FQuaternion operator+ (const FQuaternion& Q1, const FQuaternion& Q2) noexcept;
FQuaternion operator- (const FQuaternion& Q1, const FQuaternion& Q2) noexcept;
FQuaternion operator* (const FQuaternion& Q1, const FQuaternion& Q2) noexcept;
FQuaternion operator* (const FQuaternion& Q, float S) noexcept;
FQuaternion operator/ (const FQuaternion& Q1, const FQuaternion& Q2) noexcept;
FQuaternion operator* (float S, const FQuaternion& Q) noexcept;

//------------------------------------------------------------------------------
// FColor
struct FColor : public XMFLOAT4
{
    FColor() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
    constexpr FColor(float _r, float _g, float _b) noexcept : XMFLOAT4(_r, _g, _b, 1.f) {}
    constexpr FColor(float _r, float _g, float _b, float _a) noexcept : XMFLOAT4(_r, _g, _b, _a) {}
    explicit FColor(const FVector3& clr) noexcept : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
    explicit FColor(const FVector4& clr) noexcept : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
    explicit FColor(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FColor(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FColor(const XMFLOAT4& c) noexcept { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
    explicit FColor(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    // BGRA Direct3D 9 D3DCOLOR packed color
    explicit FColor(const DirectX::PackedVector::XMCOLOR& Packed) noexcept;

    // RGBA XNA Game Studio packed color
    explicit FColor(const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept;

    FColor(const FColor&) = default;
    FColor& operator=(const FColor&) = default;

    FColor(FColor&&) = default;
    FColor& operator=(FColor&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }
    operator const float* () const noexcept { return reinterpret_cast<const float*>(this); }

    // Comparison operators
    bool operator == (const FColor& c) const noexcept;
    bool operator != (const FColor& c) const noexcept;

    // Assignment operators
    FColor& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
    FColor& operator= (const DirectX::PackedVector::XMCOLOR& Packed) noexcept;
    FColor& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept;
    FColor& operator+= (const FColor& c) noexcept;
    FColor& operator-= (const FColor& c) noexcept;
    FColor& operator*= (const FColor& c) noexcept;
    FColor& operator*= (float S) noexcept;
    FColor& operator/= (const FColor& c) noexcept;

    // Unary operators
    FColor operator+ () const noexcept { return *this; }
    FColor operator- () const noexcept;

    // Properties
    float R() const noexcept { return x; }
    void R(float r) noexcept { x = r; }

    float G() const noexcept { return y; }
    void G(float g) noexcept { y = g; }

    float B() const noexcept { return z; }
    void B(float b) noexcept { z = b; }

    float A() const noexcept { return w; }
    void A(float a) noexcept { w = a; }

    // FColor operations
    DirectX::PackedVector::XMCOLOR BGRA() const noexcept;
    DirectX::PackedVector::XMUBYTEN4 RGBA() const noexcept;

    FVector3 ToVector3() const noexcept;
    FVector4 ToVector4() const noexcept;

    void Negate() noexcept;
    void Negate(FColor& result) const noexcept;

    void Saturate() noexcept;
    void Saturate(FColor& result) const noexcept;

    void Premultiply() noexcept;
    void Premultiply(FColor& result) const noexcept;

    void AdjustSaturation(float sat) noexcept;
    void AdjustSaturation(float sat, FColor& result) const noexcept;

    void AdjustContrast(float contrast) noexcept;
    void AdjustContrast(float contrast, FColor& result) const noexcept;

    // Static functions
    static void Modulate(const FColor& c1, const FColor& c2, FColor& result) noexcept;
    static FColor Modulate(const FColor& c1, const FColor& c2) noexcept;

    static void Lerp(const FColor& c1, const FColor& c2, float t, FColor& result) noexcept;
    static FColor Lerp(const FColor& c1, const FColor& c2, float t) noexcept;
};

// Binary operators
FColor operator+ (const FColor& C1, const FColor& C2) noexcept;
FColor operator- (const FColor& C1, const FColor& C2) noexcept;
FColor operator* (const FColor& C1, const FColor& C2) noexcept;
FColor operator* (const FColor& C, float S) noexcept;
FColor operator/ (const FColor& C1, const FColor& C2) noexcept;
FColor operator* (float S, const FColor& C) noexcept;

//------------------------------------------------------------------------------
// FRay
class FRay
{
public:
    FVector3 position;
    FVector3 direction;

    FRay() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
    FRay(const FVector3& pos, const FVector3& dir) noexcept : position(pos), direction(dir) {}

    FRay(const FRay&) = default;
    FRay& operator=(const FRay&) = default;

    FRay(FRay&&) = default;
    FRay& operator=(FRay&&) = default;

    // Comparison operators
    bool operator == (const FRay& r) const noexcept;
    bool operator != (const FRay& r) const noexcept;

    // FRay operations
    bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const noexcept;
    bool Intersects(const BoundingBox& box, _Out_ float& Dist) const noexcept;
    bool Intersects(const FVector3& tri0, const FVector3& tri1, const FVector3& tri2, _Out_ float& Dist) const noexcept;
    bool Intersects(const FPlane& plane, _Out_ float& Dist) const noexcept;
};

//------------------------------------------------------------------------------
// FViewport
class FViewport
{
public:
    float x;
    float y;
    float width;
    float height;
    float minDepth;
    float maxDepth;

    FViewport() noexcept :
        x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f)
    {
    }
    constexpr FViewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) noexcept :
        x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz)
    {
    }
    explicit FViewport(const RECT& rct) noexcept :
        x(float(rct.left)), y(float(rct.top)),
        width(float(rct.right - rct.left)),
        height(float(rct.bottom - rct.top)),
        minDepth(0.f), maxDepth(1.f)
    {
    }

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
    // Direct3D 11 interop
    explicit FViewport(const D3D11_VIEWPORT& vp) noexcept :
        x(vp.TopLeftX), y(vp.TopLeftY),
        width(vp.Width), height(vp.Height),
        minDepth(vp.MinDepth), maxDepth(vp.MaxDepth)
    {
    }

    operator D3D11_VIEWPORT() noexcept { return *reinterpret_cast<const D3D11_VIEWPORT*>(this); }
    const D3D11_VIEWPORT* Get11() const noexcept { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }
    FViewport& operator= (const D3D11_VIEWPORT& vp) noexcept;
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
    // Direct3D 12 interop
    explicit FViewport(const D3D12_VIEWPORT& vp) noexcept :
        x(vp.TopLeftX), y(vp.TopLeftY),
        width(vp.Width), height(vp.Height),
        minDepth(vp.MinDepth), maxDepth(vp.MaxDepth)
    {
    }

    operator D3D12_VIEWPORT() noexcept { return *reinterpret_cast<const D3D12_VIEWPORT*>(this); }
    const D3D12_VIEWPORT* Get12() const noexcept { return reinterpret_cast<const D3D12_VIEWPORT*>(this); }
    FViewport& operator= (const D3D12_VIEWPORT& vp) noexcept;
#endif

    FViewport(const FViewport&) = default;
    FViewport& operator=(const FViewport&) = default;

    FViewport(FViewport&&) = default;
    FViewport& operator=(FViewport&&) = default;

    // Comparison operators
#if (__cplusplus >= 202002L)
    bool operator == (const FViewport&) const = default;
    auto operator <=> (const FViewport&) const = default;
#else
    bool operator == (const FViewport& vp) const noexcept;
    bool operator != (const FViewport& vp) const noexcept;
#endif

    // Assignment operators
    FViewport& operator= (const RECT& rct) noexcept;

    // FViewport operations
    float AspectRatio() const noexcept;

    FVector3 Project(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world) const noexcept;
    void Project(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world, FVector3& result) const noexcept;

    FVector3 Unproject(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world) const noexcept;
    void Unproject(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world, FVector3& result) const noexcept;

    // Static methods
#if defined(__dxgi1_2_h__) || defined(__d3d11_x_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
    static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight) noexcept;
#endif
    static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight) noexcept;
};


