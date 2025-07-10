#include "Common/Math.h"


const FVector2 FVector2::Zero = { 0.f, 0.f };
const FVector2 FVector2::One = { 1.f, 1.f };
const FVector2 FVector2::UnitX = { 1.f, 0.f };
const FVector2 FVector2::UnitY = { 0.f, 1.f };

const FVector3 FVector3::Zero = { 0.f, 0.f, 0.f };
const FVector3 FVector3::One = { 1.f, 1.f, 1.f };
const FVector3 FVector3::UnitX = { 1.f, 0.f, 0.f };
const FVector3 FVector3::UnitY = { 0.f, 1.f, 0.f };
const FVector3 FVector3::UnitZ = { 0.f, 0.f, 1.f };
const FVector3 FVector3::Up = { 0.f, 1.f, 0.f };
const FVector3 FVector3::Down = { 0.f, -1.f, 0.f };
const FVector3 FVector3::Right = { 1.f, 0.f, 0.f };
const FVector3 FVector3::Left = { -1.f, 0.f, 0.f };
const FVector3 FVector3::Forward = { 0.f, 0.f, 1.f };
const FVector3 FVector3::Backward = { 0.f, 0.f, -1.f };

const FVector4 FVector4::Zero = { 0.f, 0.f, 0.f, 0.f };
const FVector4 FVector4::One = { 1.f, 1.f, 1.f, 1.f };
const FVector4 FVector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
const FVector4 FVector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
const FVector4 FVector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
const FVector4 FVector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

const FMatrix FMatrix::Identity = { 1.f, 0.f, 0.f, 0.f,
									0.f, 1.f, 0.f, 0.f,
									0.f, 0.f, 1.f, 0.f,
									0.f, 0.f, 0.f, 1.f };

const FQuaternion FQuaternion::Identity = { 0.f, 0.f, 0.f, 1.f };



using namespace DirectX;
//using namespace ya::math;

/****************************************************************************
	*
	* FQuaternion
	*
	****************************************************************************/

void FQuaternion::RotateTowards(const FQuaternion& target, float maxAngle, FQuaternion& result) const noexcept
{
	const XMVECTOR T = XMLoadFloat4(this);

	// We can use the conjugate here instead of inverse assuming q1 & q2 are normalized.
	const XMVECTOR R = XMQuaternionMultiply(XMQuaternionConjugate(T), target);

	const float rs = XMVectorGetW(R);
	const XMVECTOR L = XMVector3Length(R);
	const float angle = 2.f * atan2f(XMVectorGetX(L), rs);
	if (angle > maxAngle)
	{
		const XMVECTOR delta = XMQuaternionRotationAxis(R, maxAngle);
		const XMVECTOR Q = XMQuaternionMultiply(delta, T);
		XMStoreFloat4(&result, Q);
	}
	else
	{
		// Don't overshoot.
		result = target;
	}
}

void FQuaternion::FromToRotation(const FVector3& fromDir, const FVector3& toDir, FQuaternion& result) noexcept
{
	// Melax, "The Shortest Arc FQuaternion", Game Programming Gems, Charles River Media (2000).

	const XMVECTOR F = XMVector3Normalize(fromDir);
	const XMVECTOR T = XMVector3Normalize(toDir);

	const float dot = XMVectorGetX(XMVector3Dot(F, T));
	if (dot >= 1.f)
	{
		result = Identity;
	}
	else if (dot <= -1.f)
	{
		XMVECTOR axis = XMVector3Cross(F, FVector3::Right);
		if (XMVector3NearEqual(XMVector3LengthSq(axis), g_XMZero, g_XMEpsilon))
		{
			axis = XMVector3Cross(F, FVector3::Up);
		}

		const XMVECTOR Q = XMQuaternionRotationAxis(axis, XM_PI);
		XMStoreFloat4(&result, Q);
	}
	else
	{
		const XMVECTOR C = XMVector3Cross(F, T);
		XMStoreFloat4(&result, C);

		const float s = sqrtf((1.f + dot) * 2.f);
		result.x /= s;
		result.y /= s;
		result.z /= s;
		result.w = s * 0.5f;
	}
}

void FQuaternion::LookRotation(const FVector3& forward, const FVector3& up, FQuaternion& result) noexcept
{
	FQuaternion q1;
	FromToRotation(FVector3::Forward, forward, q1);

	const XMVECTOR C = XMVector3Cross(forward, up);
	if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
	{
		// forward and up are co-linear
		result = q1;
		return;
	}

	const XMVECTOR U = XMQuaternionMultiply(q1, FVector3::Up);

	FQuaternion q2;
	FromToRotation(U, up, q2);

	XMStoreFloat4(&result, XMQuaternionMultiply(q2, q1));
}


/****************************************************************************
*
* FViewport
*
****************************************************************************/

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
static_assert(sizeof(DirectX::SimpleMath::FViewport) == sizeof(D3D11_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, x) == offsetof(D3D11_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, y) == offsetof(D3D11_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, width) == offsetof(D3D11_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, height) == offsetof(D3D11_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, minDepth) == offsetof(D3D11_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, maxDepth) == offsetof(D3D11_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
static_assert(sizeof(DirectX::SimpleMath::FViewport) == sizeof(D3D12_VIEWPORT), "Size mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, x) == offsetof(D3D12_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, y) == offsetof(D3D12_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, width) == offsetof(D3D12_VIEWPORT, Width), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, height) == offsetof(D3D12_VIEWPORT, Height), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, minDepth) == offsetof(D3D12_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(offsetof(DirectX::SimpleMath::FViewport, maxDepth) == offsetof(D3D12_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

#if defined(__dxgi1_2_h__) || defined(__d3d11_x_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
RECT FViewport::ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight) noexcept
{
	RECT rct = {};

	switch (int(scaling))
	{
	case DXGI_SCALING_STRETCH:
		// Output fills the entire window area
		rct.top = 0;
		rct.left = 0;
		rct.right = outputWidth;
		rct.bottom = outputHeight;
		break;

	case 2 /*DXGI_SCALING_ASPECT_RATIO_STRETCH*/:
		// Output fills the window area but respects the original aspect ratio, using pillar boxing or letter boxing as required
		// Note: This scaling option is not supported for legacy Win32 windows swap chains
	{
		assert(backBufferHeight > 0);
		const float aspectRatio = float(backBufferWidth) / float(backBufferHeight);

		// Horizontal fill
		float scaledWidth = float(outputWidth);
		float scaledHeight = float(outputWidth) / aspectRatio;
		if (scaledHeight >= float(outputHeight))
		{
			// Do vertical fill
			scaledWidth = float(outputHeight) * aspectRatio;
			scaledHeight = float(outputHeight);
		}

		const float offsetX = (float(outputWidth) - scaledWidth) * 0.5f;
		const float offsetY = (float(outputHeight) - scaledHeight) * 0.5f;

		rct.left = static_cast<LONG>(offsetX);
		rct.top = static_cast<LONG>(offsetY);
		rct.right = static_cast<LONG>(offsetX + scaledWidth);
		rct.bottom = static_cast<LONG>(offsetY + scaledHeight);

		// Clip to display window
		rct.left = std::max<LONG>(0, rct.left);
		rct.top = std::max<LONG>(0, rct.top);
		rct.right = std::min<LONG>(outputWidth, rct.right);
		rct.bottom = std::min<LONG>(outputHeight, rct.bottom);
	}
	break;

	case DXGI_SCALING_NONE:
	default:
		// Output is displayed in the upper left corner of the window area
		rct.top = 0;
		rct.left = 0;
		rct.right = std::min<LONG>(static_cast<LONG>(backBufferWidth), outputWidth);
		rct.bottom = std::min<LONG>(static_cast<LONG>(backBufferHeight), outputHeight);
		break;
	}

	return rct;
}
#endif

RECT FViewport::ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight) noexcept
{
	const float safew = (float(backBufferWidth) + 19.f) / 20.f;
	const float safeh = (float(backBufferHeight) + 19.f) / 20.f;

	RECT rct;
	rct.left = static_cast<LONG>(safew);
	rct.top = static_cast<LONG>(safeh);
	rct.right = static_cast<LONG>(float(backBufferWidth) - safew + 0.5f);
	rct.bottom = static_cast<LONG>(float(backBufferHeight) - safeh + 0.5f);

	return rct;
}

FVector2 FRectangle::Location() const noexcept
{
	return FVector2(float(x), float(y));
}

FVector2 FRectangle::Center() const noexcept
{
	return FVector2(float(x) + (float(width) / 2.f), float(y) + (float(height) / 2.f));
}

bool FRectangle::Contains(const FVector2& point) const noexcept
{
	return (float(x) <= point.x) && (point.x < float(x + width)) && (float(y) <= point.y) && (point.y < float(y + height));
}

void FRectangle::Inflate(long horizAmount, long vertAmount) noexcept
{
	x -= horizAmount;
	y -= vertAmount;
	width += horizAmount;
	height += vertAmount;
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

FRectangle FRectangle::Intersect(const FRectangle& ra, const FRectangle& rb) noexcept
{
	const long righta = ra.x + ra.width;
	const long rightb = rb.x + rb.width;

	const long bottoma = ra.y + ra.height;
	const long bottomb = rb.y + rb.height;

	const long maxX = ra.x > rb.x ? ra.x : rb.x;
	const long maxY = ra.y > rb.y ? ra.y : rb.y;

	const long minRight = righta < rightb ? righta : rightb;
	const long minBottom = bottoma < bottomb ? bottoma : bottomb;

	FRectangle result;

	if ((minRight > maxX) && (minBottom > maxY))
	{
		result.x = maxX;
		result.y = maxY;
		result.width = minRight - maxX;
		result.height = minBottom - maxY;
	}
	else
	{
		result.x = 0;
		result.y = 0;
		result.width = 0;
		result.height = 0;
	}

	return result;
}

RECT FRectangle::Intersect(const RECT& rcta, const RECT& rctb) noexcept
{
	const long maxX = rcta.left > rctb.left ? rcta.left : rctb.left;
	const long maxY = rcta.top > rctb.top ? rcta.top : rctb.top;

	const long minRight = rcta.right < rctb.right ? rcta.right : rctb.right;
	const long minBottom = rcta.bottom < rctb.bottom ? rcta.bottom : rctb.bottom;

	RECT result;

	if ((minRight > maxX) && (minBottom > maxY))
	{
		result.left = maxX;
		result.top = maxY;
		result.right = minRight;
		result.bottom = minBottom;
	}
	else
	{
		result.left = 0;
		result.top = 0;
		result.right = 0;
		result.bottom = 0;
	}

	return result;
}

FRectangle FRectangle::Union(const FRectangle& ra, const FRectangle& rb) noexcept
{
	const long righta = ra.x + ra.width;
	const long rightb = rb.x + rb.width;

	const long bottoma = ra.y + ra.height;
	const long bottomb = rb.y + rb.height;

	const int minX = ra.x < rb.x ? ra.x : rb.x;
	const int minY = ra.y < rb.y ? ra.y : rb.y;

	const int maxRight = righta > rightb ? righta : rightb;
	const int maxBottom = bottoma > bottomb ? bottoma : bottomb;

	FRectangle result;
	result.x = minX;
	result.y = minY;
	result.width = maxRight - minX;
	result.height = maxBottom - minY;
	return result;
}

RECT FRectangle::Union(const RECT& rcta, const RECT& rctb) noexcept
{
	RECT result;
	result.left = rcta.left < rctb.left ? rcta.left : rctb.left;
	result.top = rcta.top < rctb.top ? rcta.top : rctb.top;
	result.right = rcta.right > rctb.right ? rcta.right : rctb.right;
	result.bottom = rcta.bottom > rctb.bottom ? rcta.bottom : rctb.bottom;
	return result;
}


/****************************************************************************
	*
	* FVector2
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

bool FVector2::operator == (const FVector2& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2Equal(v1, v2);
}

bool FVector2::operator != (const FVector2& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FVector2& FVector2::operator+= (const FVector2& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

FVector2& FVector2::operator-= (const FVector2& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

FVector2& FVector2::operator*= (const FVector2& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

FVector2& FVector2::operator*= (float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat2(this, X);
	return *this;
}

FVector2& FVector2::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat2(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FVector2 operator+ (const FVector2& V1, const FVector2& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V1);
	const XMVECTOR v2 = XMLoadFloat2(&V2);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator- (const FVector2& V1, const FVector2& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V1);
	const XMVECTOR v2 = XMLoadFloat2(&V2);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator* (const FVector2& V1, const FVector2& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V1);
	const XMVECTOR v2 = XMLoadFloat2(&V2);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator* (const FVector2& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator/ (const FVector2& V1, const FVector2& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V1);
	const XMVECTOR v2 = XMLoadFloat2(&V2);
	const XMVECTOR X = XMVectorDivide(v1, v2);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator/ (const FVector2& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

FVector2 operator* (float S, const FVector2& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool FVector2::InBounds(const FVector2& Bounds) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&Bounds);
	return XMVector2InBounds(v1, v2);
}

float FVector2::Length() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVector2Length(v1);
	return XMVectorGetX(X);
}

float FVector2::LengthSquared() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVector2LengthSq(v1);
	return XMVectorGetX(X);
}

float FVector2::Dot(const FVector2& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR X = XMVector2Dot(v1, v2);
	return XMVectorGetX(X);
}

void FVector2::Cross(const FVector2& V, FVector2& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR R = XMVector2Cross(v1, v2);
	XMStoreFloat2(&result, R);
}

FVector2 FVector2::Cross(const FVector2& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&V);
	const XMVECTOR R = XMVector2Cross(v1, v2);

	FVector2 result;
	XMStoreFloat2(&result, R);
	return result;
}

void FVector2::Normalize() noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVector2Normalize(v1);
	XMStoreFloat2(this, X);
}

void FVector2::Normalize(FVector2& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR X = XMVector2Normalize(v1);
	XMStoreFloat2(&result, X);
}

void FVector2::Clamp(const FVector2& vmin, const FVector2& vmax) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&vmin);
	const XMVECTOR v3 = XMLoadFloat2(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat2(this, X);
}

void FVector2::Clamp(const FVector2& vmin, const FVector2& vmax, FVector2& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(this);
	const XMVECTOR v2 = XMLoadFloat2(&vmin);
	const XMVECTOR v3 = XMLoadFloat2(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat2(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float FVector2::Distance(const FVector2& v1, const FVector2& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector2Length(V);
	return XMVectorGetX(X);
}

float FVector2::DistanceSquared(const FVector2& v1, const FVector2& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector2LengthSq(V);
	return XMVectorGetX(X);
}

void FVector2::Min(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Min(const FVector2& v1, const FVector2& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Max(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Max(const FVector2& v1, const FVector2& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Lerp(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Lerp(const FVector2& v1, const FVector2& v2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::SmoothStep(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::SmoothStep(const FVector2& v1, const FVector2& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR x3 = XMLoadFloat2(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR x3 = XMLoadFloat2(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR x3 = XMLoadFloat2(&v3);
	const XMVECTOR x4 = XMLoadFloat2(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&v2);
	const XMVECTOR x3 = XMLoadFloat2(&v3);
	const XMVECTOR x4 = XMLoadFloat2(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&t1);
	const XMVECTOR x3 = XMLoadFloat2(&v2);
	const XMVECTOR x4 = XMLoadFloat2(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat2(&v1);
	const XMVECTOR x2 = XMLoadFloat2(&t1);
	const XMVECTOR x3 = XMLoadFloat2(&v2);
	const XMVECTOR x4 = XMLoadFloat2(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Reflect(const FVector2& ivec, const FVector2& nvec, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat2(&ivec);
	const XMVECTOR n = XMLoadFloat2(&nvec);
	const XMVECTOR X = XMVector2Reflect(i, n);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Reflect(const FVector2& ivec, const FVector2& nvec) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat2(&ivec);
	const XMVECTOR n = XMLoadFloat2(&nvec);
	const XMVECTOR X = XMVector2Reflect(i, n);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat2(&ivec);
	const XMVECTOR n = XMLoadFloat2(&nvec);
	const XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat2(&ivec);
	const XMVECTOR n = XMLoadFloat2(&nvec);
	const XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::JTransform(const FVector2& v, const FQuaternion& quat, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	const XMVECTOR X = XMVector3Rotate(v1, q);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::JTransform(const FVector2& v, const FQuaternion& quat) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	const XMVECTOR X = XMVector3Rotate(v1, q);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void FVector2::JTransform(const FVector2& v, const FMatrix& m, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector2TransformCoord(v1, M);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::JTransform(const FVector2& v, const FMatrix& m) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector2TransformCoord(v1, M);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

_Use_decl_annotations_
	void FVector2::JTransform(const FVector2* varray, size_t count, const FMatrix& m, FVector2* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}

void FVector2::JTransform(const FVector2& v, const FMatrix& m, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector2Transform(v1, M);
	XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
	void FVector2::JTransform(const FVector2* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
}

void FVector2::TransformNormal(const FVector2& v, const FMatrix& m, FVector2& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector2TransformNormal(v1, M);
	XMStoreFloat2(&result, X);
}

FVector2 FVector2::TransformNormal(const FVector2& v, const FMatrix& m) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector2TransformNormal(v1, M);

	FVector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

_Use_decl_annotations_
	void FVector2::TransformNormal(const FVector2* varray, size_t count, const FMatrix& m, FVector2* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}


/****************************************************************************
	*
	* FVector3
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

bool FVector3::operator == (const FVector3& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3Equal(v1, v2);
}

bool FVector3::operator != (const FVector3& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FVector3& FVector3::operator+= (const FVector3& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

FVector3& FVector3::operator-= (const FVector3& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

FVector3& FVector3::operator*= (const FVector3& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

FVector3& FVector3::operator*= (float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat3(this, X);
	return *this;
}

FVector3& FVector3::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat3(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

FVector3 FVector3::operator- () const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVectorNegate(v1);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FVector3 operator+ (const FVector3& V1, const FVector3& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V1);
	const XMVECTOR v2 = XMLoadFloat3(&V2);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator- (const FVector3& V1, const FVector3& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V1);
	const XMVECTOR v2 = XMLoadFloat3(&V2);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator* (const FVector3& V1, const FVector3& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V1);
	const XMVECTOR v2 = XMLoadFloat3(&V2);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator* (const FVector3& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator/ (const FVector3& V1, const FVector3& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V1);
	const XMVECTOR v2 = XMLoadFloat3(&V2);
	const XMVECTOR X = XMVectorDivide(v1, v2);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator/ (const FVector3& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

FVector3 operator* (float S, const FVector3& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool FVector3::InBounds(const FVector3& Bounds) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&Bounds);
	return XMVector3InBounds(v1, v2);
}

float FVector3::Length() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVector3Length(v1);
	return XMVectorGetX(X);
}

float FVector3::LengthSquared() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVector3LengthSq(v1);
	return XMVectorGetX(X);
}

float FVector3::Dot(const FVector3& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR X = XMVector3Dot(v1, v2);
	return XMVectorGetX(X);
}

void FVector3::Cross(const FVector3& V, FVector3& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR R = XMVector3Cross(v1, v2);
	XMStoreFloat3(&result, R);
}

FVector3 FVector3::Cross(const FVector3& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&V);
	const XMVECTOR R = XMVector3Cross(v1, v2);

	FVector3 result;
	XMStoreFloat3(&result, R);
	return result;
}

void FVector3::Normalize() noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVector3Normalize(v1);
	XMStoreFloat3(this, X);
}

void FVector3::Normalize(FVector3& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR X = XMVector3Normalize(v1);
	XMStoreFloat3(&result, X);
}

void FVector3::Clamp(const FVector3& vmin, const FVector3& vmax) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&vmin);
	const XMVECTOR v3 = XMLoadFloat3(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat3(this, X);
}

void FVector3::Clamp(const FVector3& vmin, const FVector3& vmax, FVector3& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(this);
	const XMVECTOR v2 = XMLoadFloat3(&vmin);
	const XMVECTOR v3 = XMLoadFloat3(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat3(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float FVector3::Distance(const FVector3& v1, const FVector3& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector3Length(V);
	return XMVectorGetX(X);
}

float FVector3::DistanceSquared(const FVector3& v1, const FVector3& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector3LengthSq(V);
	return XMVectorGetX(X);
}

void FVector3::Min(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Min(const FVector3& v1, const FVector3& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Max(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Max(const FVector3& v1, const FVector3& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Lerp(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Lerp(const FVector3& v1, const FVector3& v2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::SmoothStep(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::SmoothStep(const FVector3& v1, const FVector3& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR x3 = XMLoadFloat3(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR x3 = XMLoadFloat3(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR x3 = XMLoadFloat3(&v3);
	const XMVECTOR x4 = XMLoadFloat3(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&v2);
	const XMVECTOR x3 = XMLoadFloat3(&v3);
	const XMVECTOR x4 = XMLoadFloat3(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&t1);
	const XMVECTOR x3 = XMLoadFloat3(&v2);
	const XMVECTOR x4 = XMLoadFloat3(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat3(&v1);
	const XMVECTOR x2 = XMLoadFloat3(&t1);
	const XMVECTOR x3 = XMLoadFloat3(&v2);
	const XMVECTOR x4 = XMLoadFloat3(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Reflect(const FVector3& ivec, const FVector3& nvec, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat3(&ivec);
	const XMVECTOR n = XMLoadFloat3(&nvec);
	const XMVECTOR X = XMVector3Reflect(i, n);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Reflect(const FVector3& ivec, const FVector3& nvec) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat3(&ivec);
	const XMVECTOR n = XMLoadFloat3(&nvec);
	const XMVECTOR X = XMVector3Reflect(i, n);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat3(&ivec);
	const XMVECTOR n = XMLoadFloat3(&nvec);
	const XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat3(&ivec);
	const XMVECTOR n = XMLoadFloat3(&nvec);
	const XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::JTransform(const FVector3& v, const FQuaternion& quat, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	const XMVECTOR X = XMVector3Rotate(v1, q);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::JTransform(const FVector3& v, const FQuaternion& quat) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	const XMVECTOR X = XMVector3Rotate(v1, q);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void FVector3::JTransform(const FVector3& v, const FMatrix& m, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector3TransformCoord(v1, M);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::JTransform(const FVector3& v, const FMatrix& m) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector3TransformCoord(v1, M);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

_Use_decl_annotations_
	void FVector3::JTransform(const FVector3* varray, size_t count, const FMatrix& m, FVector3* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}

void FVector3::JTransform(const FVector3& v, const FMatrix& m, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector3Transform(v1, M);
	XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
	void FVector3::JTransform(const FVector3* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
}

void FVector3::TransformNormal(const FVector3& v, const FMatrix& m, FVector3& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector3TransformNormal(v1, M);
	XMStoreFloat3(&result, X);
}

FVector3 FVector3::TransformNormal(const FVector3& v, const FMatrix& m) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector3TransformNormal(v1, M);

	FVector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

_Use_decl_annotations_
	void FVector3::TransformNormal(const FVector3* varray, size_t count, const FMatrix& m, FVector3* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}


/****************************************************************************
	*
	* FVector4
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

bool FVector4::operator == (const FVector4& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4Equal(v1, v2);
}

bool FVector4::operator != (const FVector4& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FVector4& FVector4::operator+= (const FVector4& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

FVector4& FVector4::operator-= (const FVector4& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

FVector4& FVector4::operator*= (const FVector4& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

FVector4& FVector4::operator*= (float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat4(this, X);
	return *this;
}

FVector4& FVector4::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat4(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

FVector4 FVector4::operator- () const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVectorNegate(v1);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FVector4 operator+ (const FVector4& V1, const FVector4& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V1);
	const XMVECTOR v2 = XMLoadFloat4(&V2);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator- (const FVector4& V1, const FVector4& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V1);
	const XMVECTOR v2 = XMLoadFloat4(&V2);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator* (const FVector4& V1, const FVector4& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V1);
	const XMVECTOR v2 = XMLoadFloat4(&V2);
	const XMVECTOR X = XMVectorMultiply(v1, v2);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator* (const FVector4& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator/ (const FVector4& V1, const FVector4& V2) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V1);
	const XMVECTOR v2 = XMLoadFloat4(&V2);
	const XMVECTOR X = XMVectorDivide(v1, v2);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator/ (const FVector4& V, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorScale(v1, 1.f / S);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

FVector4 operator* (float S, const FVector4& V) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVectorScale(v1, S);
	FVector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool FVector4::InBounds(const FVector4& Bounds) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&Bounds);
	return XMVector4InBounds(v1, v2);
}

float FVector4::Length() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVector4Length(v1);
	return XMVectorGetX(X);
}

float FVector4::LengthSquared() const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVector4LengthSq(v1);
	return XMVectorGetX(X);
}

float FVector4::Dot(const FVector4& V) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&V);
	const XMVECTOR X = XMVector4Dot(v1, v2);
	return XMVectorGetX(X);
}

void FVector4::Cross(const FVector4& v1, const FVector4& v2, FVector4& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(this);
	const XMVECTOR x2 = XMLoadFloat4(&v1);
	const XMVECTOR x3 = XMLoadFloat4(&v2);
	const XMVECTOR R = XMVector4Cross(x1, x2, x3);
	XMStoreFloat4(&result, R);
}

FVector4 FVector4::Cross(const FVector4& v1, const FVector4& v2) const noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(this);
	const XMVECTOR x2 = XMLoadFloat4(&v1);
	const XMVECTOR x3 = XMLoadFloat4(&v2);
	const XMVECTOR R = XMVector4Cross(x1, x2, x3);

	FVector4 result;
	XMStoreFloat4(&result, R);
	return result;
}

void FVector4::Normalize() noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVector4Normalize(v1);
	XMStoreFloat4(this, X);
}

void FVector4::Normalize(FVector4& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR X = XMVector4Normalize(v1);
	XMStoreFloat4(&result, X);
}

void FVector4::Clamp(const FVector4& vmin, const FVector4& vmax) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&vmin);
	const XMVECTOR v3 = XMLoadFloat4(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat4(this, X);
}

void FVector4::Clamp(const FVector4& vmin, const FVector4& vmax, FVector4& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(this);
	const XMVECTOR v2 = XMLoadFloat4(&vmin);
	const XMVECTOR v3 = XMLoadFloat4(&vmax);
	const XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat4(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float FVector4::Distance(const FVector4& v1, const FVector4& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector4Length(V);
	return XMVectorGetX(X);
}

float FVector4::DistanceSquared(const FVector4& v1, const FVector4& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR V = XMVectorSubtract(x2, x1);
	const XMVECTOR X = XMVector4LengthSq(V);
	return XMVectorGetX(X);
}

void FVector4::Min(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Min(const FVector4& v1, const FVector4& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorMin(x1, x2);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Max(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Max(const FVector4& v1, const FVector4& v2) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorMax(x1, x2);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Lerp(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Lerp(const FVector4& v1, const FVector4& v2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::SmoothStep(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::SmoothStep(const FVector4& v1, const FVector4& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR X = XMVectorLerp(x1, x2, t);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR x3 = XMLoadFloat4(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR x3 = XMLoadFloat4(&v3);
	const XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR x3 = XMLoadFloat4(&v3);
	const XMVECTOR x4 = XMLoadFloat4(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&v2);
	const XMVECTOR x3 = XMLoadFloat4(&v3);
	const XMVECTOR x4 = XMLoadFloat4(&v4);
	const XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&t1);
	const XMVECTOR x3 = XMLoadFloat4(&v2);
	const XMVECTOR x4 = XMLoadFloat4(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(&v1);
	const XMVECTOR x2 = XMLoadFloat4(&t1);
	const XMVECTOR x3 = XMLoadFloat4(&v2);
	const XMVECTOR x4 = XMLoadFloat4(&t2);
	const XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Reflect(const FVector4& ivec, const FVector4& nvec, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat4(&ivec);
	const XMVECTOR n = XMLoadFloat4(&nvec);
	const XMVECTOR X = XMVector4Reflect(i, n);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Reflect(const FVector4& ivec, const FVector4& nvec) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat4(&ivec);
	const XMVECTOR n = XMLoadFloat4(&nvec);
	const XMVECTOR X = XMVector4Reflect(i, n);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat4(&ivec);
	const XMVECTOR n = XMLoadFloat4(&nvec);
	const XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	const XMVECTOR i = XMLoadFloat4(&ivec);
	const XMVECTOR n = XMLoadFloat4(&nvec);
	const XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::JTransform(const FVector2& v, const FQuaternion& quat, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::JTransform(const FVector2& v, const FQuaternion& quat) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat2(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::JTransform(const FVector3& v, const FQuaternion& quat, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::JTransform(const FVector3& v, const FQuaternion& quat) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat3(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::JTransform(const FVector4& v, const FQuaternion& quat, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::JTransform(const FVector4& v, const FQuaternion& quat) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&v);
	const XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void FVector4::JTransform(const FVector4& v, const FMatrix& m, FVector4& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector4Transform(v1, M);
	XMStoreFloat4(&result, X);
}

FVector4 FVector4::JTransform(const FVector4& v, const FMatrix& m) noexcept
{
	using namespace DirectX;
	const XMVECTOR v1 = XMLoadFloat4(&v);
	const XMMATRIX M = XMLoadFloat4x4(&m);
	const XMVECTOR X = XMVector4Transform(v1, M);

	FVector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

_Use_decl_annotations_
	void FVector4::JTransform(const FVector4* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
}


/****************************************************************************
	*
	* FMatrix
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

bool FMatrix::operator == (const FMatrix& M) const noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	const XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	const XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	const XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	return (XMVector4Equal(x1, y1)
		&& XMVector4Equal(x2, y2)
		&& XMVector4Equal(x3, y3)
		&& XMVector4Equal(x4, y4)) != 0;
}

bool FMatrix::operator != (const FMatrix& M) const noexcept
{
	using namespace DirectX;
	const XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	const XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	const XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	const XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	return (XMVector4NotEqual(x1, y1)
		|| XMVector4NotEqual(x2, y2)
		|| XMVector4NotEqual(x3, y3)
		|| XMVector4NotEqual(x4, y4)) != 0;
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FMatrix::FMatrix(const XMFLOAT3X3& M) noexcept
{
	_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
	_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
	_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
	_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
}

FMatrix::FMatrix(const XMFLOAT4X3& M) noexcept
{
	_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
	_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
	_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
	_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
}

FMatrix& FMatrix::operator= (const XMFLOAT3X3& M) noexcept
{
	_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
	_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
	_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
	_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
	return *this;
}

FMatrix& FMatrix::operator= (const XMFLOAT4X3& M) noexcept
{
	_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
	_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
	_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
	_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
	return *this;
}

FMatrix& FMatrix::operator+= (const FMatrix& M) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorAdd(x1, y1);
	x2 = XMVectorAdd(x2, y2);
	x3 = XMVectorAdd(x3, y3);
	x4 = XMVectorAdd(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

FMatrix& FMatrix::operator-= (const FMatrix& M) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorSubtract(x1, y1);
	x2 = XMVectorSubtract(x2, y2);
	x3 = XMVectorSubtract(x3, y3);
	x4 = XMVectorSubtract(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

FMatrix& FMatrix::operator*= (const FMatrix& M) noexcept
{
	using namespace DirectX;
	const XMMATRIX M1 = XMLoadFloat4x4(this);
	const XMMATRIX M2 = XMLoadFloat4x4(&M);
	const XMMATRIX X = XMMatrixMultiply(M1, M2);
	XMStoreFloat4x4(this, X);
	return *this;
}

FMatrix& FMatrix::operator*= (float S) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

FMatrix& FMatrix::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.f);
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const float rs = 1.f / S;

	x1 = XMVectorScale(x1, rs);
	x2 = XMVectorScale(x2, rs);
	x3 = XMVectorScale(x3, rs);
	x4 = XMVectorScale(x4, rs);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

FMatrix& FMatrix::operator/= (const FMatrix& M) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorDivide(x1, y1);
	x2 = XMVectorDivide(x2, y2);
	x3 = XMVectorDivide(x3, y3);
	x4 = XMVectorDivide(x4, y4);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

FMatrix FMatrix::operator- () const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

	v1 = XMVectorNegate(v1);
	v2 = XMVectorNegate(v2);
	v3 = XMVectorNegate(v3);
	v4 = XMVectorNegate(v4);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FMatrix operator+ (const FMatrix& M1, const FMatrix& M2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorAdd(x1, y1);
	x2 = XMVectorAdd(x2, y2);
	x3 = XMVectorAdd(x3, y3);
	x4 = XMVectorAdd(x4, y4);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

FMatrix operator- (const FMatrix& M1, const FMatrix& M2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorSubtract(x1, y1);
	x2 = XMVectorSubtract(x2, y2);
	x3 = XMVectorSubtract(x3, y3);
	x4 = XMVectorSubtract(x4, y4);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

FMatrix operator* (const FMatrix& M1, const FMatrix& M2) noexcept
{
	using namespace DirectX;
	const XMMATRIX m1 = XMLoadFloat4x4(&M1);
	const XMMATRIX m2 = XMLoadFloat4x4(&M2);
	const XMMATRIX X = XMMatrixMultiply(m1, m2);

	FMatrix R;
	XMStoreFloat4x4(&R, X);
	return R;
}

FMatrix operator* (const FMatrix& M, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

FMatrix operator/ (const FMatrix& M, float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.f);

	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	const float rs = 1.f / S;

	x1 = XMVectorScale(x1, rs);
	x2 = XMVectorScale(x2, rs);
	x3 = XMVectorScale(x3, rs);
	x4 = XMVectorScale(x4, rs);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

FMatrix operator/ (const FMatrix& M1, const FMatrix& M2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorDivide(x1, y1);
	x2 = XMVectorDivide(x2, y2);
	x3 = XMVectorDivide(x3, y3);
	x4 = XMVectorDivide(x4, y4);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

FMatrix operator* (float S, const FMatrix& M) noexcept
{
	using namespace DirectX;

	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

	x1 = XMVectorScale(x1, S);
	x2 = XMVectorScale(x2, S);
	x3 = XMVectorScale(x3, S);
	x4 = XMVectorScale(x4, S);

	FMatrix R;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
	return R;
}

//------------------------------------------------------------------------------
// FMatrix operations
//------------------------------------------------------------------------------

bool FMatrix::Decompose(FVector3& scale, FQuaternion& rotation, FVector3& translation) noexcept
{
	using namespace DirectX;

	XMVECTOR s, r, t;

	if (!XMMatrixDecompose(&s, &r, &t, *this))
		return false;

	XMStoreFloat3(&scale, s);
	XMStoreFloat4(&rotation, r);
	XMStoreFloat3(&translation, t);

	return true;
}

FMatrix FMatrix::Transpose() const noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(this);
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixTranspose(M));
	return R;
}

void FMatrix::Transpose(FMatrix& result) const noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(this);
	XMStoreFloat4x4(&result, XMMatrixTranspose(M));
}

FMatrix FMatrix::Invert() const noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(this);
	FMatrix R;
	XMVECTOR det;
	XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
	return R;
}

void FMatrix::Invert(FMatrix& result) const noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(this);
	XMVECTOR det;
	XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
}

float FMatrix::Determinant() const noexcept
{
	using namespace DirectX;
	const XMMATRIX M = XMLoadFloat4x4(this);
	return XMVectorGetX(XMMatrixDeterminant(M));
}

FVector3 FMatrix::ToEuler() const noexcept
{
	const float cy = sqrtf(_33 * _33 + _31 * _31);
	const float cx = atan2f(-_32, cy);
	if (cy > 16.f * FLT_EPSILON)
	{
		return FVector3(cx, atan2f(_31, _33), atan2f(_12, _22));
	}
	else
	{
		return FVector3(cx, 0.f, atan2f(-_21, _11));
	}
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

_Use_decl_annotations_
	FMatrix FMatrix::CreateBillboard(
		const FVector3& object,
		const FVector3& cameraPosition,
		const FVector3& cameraUp,
		const FVector3* cameraForward) noexcept
{
	using namespace DirectX;
	const XMVECTOR O = XMLoadFloat3(&object);
	const XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR Z = XMVectorSubtract(O, C);

	const XMVECTOR N = XMVector3LengthSq(Z);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			const XMVECTOR F = XMLoadFloat3(cameraForward);
			Z = XMVectorNegate(F);
		}
		else
			Z = g_XMNegIdentityR2;
	}
	else
	{
		Z = XMVector3Normalize(Z);
	}

	const XMVECTOR up = XMLoadFloat3(&cameraUp);
	XMVECTOR X = XMVector3Cross(up, Z);
	X = XMVector3Normalize(X);

	const XMVECTOR Y = XMVector3Cross(Z, X);

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	FMatrix R;
	XMStoreFloat4x4(&R, M);
	return R;
}

_Use_decl_annotations_
	FMatrix FMatrix::CreateConstrainedBillboard(
		const FVector3& object,
		const FVector3& cameraPosition,
		const FVector3& rotateAxis,
		const FVector3* cameraForward,
		const FVector3* objectForward) noexcept
{
	using namespace DirectX;

	static const XMVECTORF32 s_minAngle = { { { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f } } }; // 1.0 - XMConvertToRadians( 0.1f );

	const XMVECTOR O = XMLoadFloat3(&object);
	const XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR faceDir = XMVectorSubtract(O, C);

	const XMVECTOR N = XMVector3LengthSq(faceDir);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			const XMVECTOR F = XMLoadFloat3(cameraForward);
			faceDir = XMVectorNegate(F);
		}
		else
			faceDir = g_XMNegIdentityR2;
	}
	else
	{
		faceDir = XMVector3Normalize(faceDir);
	}

	const XMVECTOR Y = XMLoadFloat3(&rotateAxis);
	XMVECTOR X, Z;

	XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
	if (XMVector3Greater(dot, s_minAngle))
	{
		if (objectForward)
		{
			Z = XMLoadFloat3(objectForward);
			dot = XMVectorAbs(XMVector3Dot(Y, Z));
			if (XMVector3Greater(dot, s_minAngle))
			{
				dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
				Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
			}
		}
		else
		{
			dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
			Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
		}

		X = XMVector3Cross(Y, Z);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}
	else
	{
		X = XMVector3Cross(Y, faceDir);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	FMatrix R;
	XMStoreFloat4x4(&R, M);
	return R;
}

FMatrix FMatrix::CreateTranslation(const FVector3& position) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
	return R;
}

FMatrix FMatrix::CreateTranslation(float x, float y, float z) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
	return R;
}

FMatrix FMatrix::CreateScale(const FVector3& scales) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
	return R;
}

FMatrix FMatrix::CreateScale(float xs, float ys, float zs) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
	return R;
}

FMatrix FMatrix::CreateScale(float scale) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
	return R;
}

FMatrix FMatrix::CreateRotationX(float radians) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
	return R;
}

FMatrix FMatrix::CreateRotationY(float radians) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
	return R;
}

FMatrix FMatrix::CreateRotationZ(float radians) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
	return R;
}

FMatrix FMatrix::CreateFromAxisAngle(const FVector3& axis, float angle) noexcept
{
	using namespace DirectX;
	FMatrix R;
	const XMVECTOR a = XMLoadFloat3(&axis);
	XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
	return R;
}

FMatrix FMatrix::CreatePerspectiveFieldOfViewRH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane));
	return R;
}

FMatrix FMatrix::CreatePerspectiveRH(float width, float height, float nearPlane, float farPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveRH(width, height, nearPlane, farPlane));
	return R;
}

FMatrix FMatrix::CreatePerspectiveFieldOfViewLH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept
{
	using namespace DirectX;
	FMatrix L;
	XMStoreFloat4x4(&L, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	return L;
}

FMatrix FMatrix::CreatePerspectiveLH(float width, float height, float nearPlane, float farPlane) noexcept
{
	using namespace DirectX;
	FMatrix L;
	XMStoreFloat4x4(&L, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
	return L;
}

FMatrix FMatrix::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterRH(left, right, bottom, top, nearPlane, farPlane));
	return R;
}

FMatrix FMatrix::CreateOrthographicRH(float width, float height, float zNearPlane, float zFarPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicRH(width, height, zNearPlane, zFarPlane));
	return R;
}

FMatrix FMatrix::CreateOrthographicOffCenterRH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterRH(left, right, bottom, top, zNearPlane, zFarPlane));
	return R;
}

FMatrix FMatrix::CreateOrthographicLH(float width, float height, float zNearPlane, float zFarPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
	return R;
}

FMatrix FMatrix::CreateOrthographicOffCenterLH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
	return R;
}

FMatrix FMatrix::CreateLookAtRH(const FVector3& eye, const FVector3& target, const FVector3& up) noexcept
{
	using namespace DirectX;
	FMatrix R;
	const XMVECTOR eyev = XMLoadFloat3(&eye);
	const XMVECTOR targetv = XMLoadFloat3(&target);
	const XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
	return R;
}

FMatrix FMatrix::CreateLookAtLH(const FVector3& eye, const FVector3& target, const FVector3& up) noexcept
{
	using namespace DirectX;
	FMatrix R;
	const XMVECTOR eyev = XMLoadFloat3(&eye);
	const XMVECTOR targetv = XMLoadFloat3(&target);
	const XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
	return R;
}

FMatrix FMatrix::CreateLookToRH(const FVector3& eye, const FVector3& target, const FVector3& up) noexcept
{
	using namespace DirectX;
	FMatrix R;
	const XMVECTOR eyev = XMLoadFloat3(&eye);
	const XMVECTOR targetv = XMLoadFloat3(&target);
	const XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookToRH(eyev, targetv, upv));
	return R;
}

FMatrix FMatrix::CreateLookToLH(const FVector3& eye, const FVector3& target, const FVector3& up) noexcept
{
	using namespace DirectX;
	FMatrix R;
	const XMVECTOR eyev = XMLoadFloat3(&eye);
	const XMVECTOR targetv = XMLoadFloat3(&target);
	const XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookToLH(eyev, targetv, upv));
	return R;
}

FMatrix FMatrix::CreateWorld(const FVector3& position, const FVector3& forward, const FVector3& up) noexcept
{
	using namespace DirectX;
	const XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
	XMVECTOR yaxis = XMLoadFloat3(&up);
	const XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
	yaxis = XMVector3Cross(zaxis, xaxis);

	FMatrix R;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
	R._14 = R._24 = R._34 = 0.f;
	R._41 = position.x; R._42 = position.y; R._43 = position.z;
	R._44 = 1.f;
	return R;
}

FMatrix FMatrix::CreateFromQuaternion(const FQuaternion& rotation) noexcept
{
	using namespace DirectX;
	const XMVECTOR quatv = XMLoadFloat4(&rotation);
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
	return R;
}

FMatrix FMatrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return R;
}

FMatrix FMatrix::CreateFromYawPitchRoll(const FVector3& angles) noexcept
{
	using namespace DirectX;
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYawFromVector(angles));
	return R;
}

FMatrix FMatrix::CreateShadow(const FVector3& lightDir, const FPlane& plane) noexcept
{
	using namespace DirectX;
	const XMVECTOR light = XMLoadFloat3(&lightDir);
	const XMVECTOR planev = XMLoadFloat4(&plane);
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixShadow(planev, light));
	return R;
}

FMatrix FMatrix::CreateReflection(const FPlane& plane) noexcept
{
	using namespace DirectX;
	const XMVECTOR planev = XMLoadFloat4(&plane);
	FMatrix R;
	XMStoreFloat4x4(&R, XMMatrixReflect(planev));
	return R;
}

void FMatrix::Lerp(const FMatrix& M1, const FMatrix& M2, float t, FMatrix& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorLerp(x1, y1, t);
	x2 = XMVectorLerp(x2, y2, t);
	x3 = XMVectorLerp(x3, y3, t);
	x4 = XMVectorLerp(x4, y4, t);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
}

FMatrix FMatrix::Lerp(const FMatrix& M1, const FMatrix& M2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
	XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
	XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
	XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

	const XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
	const XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
	const XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
	const XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

	x1 = XMVectorLerp(x1, y1, t);
	x2 = XMVectorLerp(x2, y2, t);
	x3 = XMVectorLerp(x3, y3, t);
	x4 = XMVectorLerp(x4, y4, t);

	FMatrix result;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
	return result;
}

void FMatrix::JTransform(const FMatrix& M, const FQuaternion& rotation, FMatrix& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR quatv = XMLoadFloat4(&rotation);

	const XMMATRIX M0 = XMLoadFloat4x4(&M);
	const XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
}

FMatrix FMatrix::JTransform(const FMatrix& M, const FQuaternion& rotation) noexcept
{
	using namespace DirectX;
	const XMVECTOR quatv = XMLoadFloat4(&rotation);

	const XMMATRIX M0 = XMLoadFloat4x4(&M);
	const XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	FMatrix result;
	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
	return result;
}


/****************************************************************************
	*
	* FPlane
	*
	****************************************************************************/

FPlane::FPlane(const FVector3& point1, const FVector3& point2, const FVector3& point3) noexcept
{
	using namespace DirectX;
	const XMVECTOR P0 = XMLoadFloat3(&point1);
	const XMVECTOR P1 = XMLoadFloat3(&point2);
	const XMVECTOR P2 = XMLoadFloat3(&point3);
	XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
}

FPlane::FPlane(const FVector3& point, const FVector3& normal) noexcept
{
	using namespace DirectX;
	const XMVECTOR P = XMLoadFloat3(&point);
	const XMVECTOR N = XMLoadFloat3(&normal);
	XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool FPlane::operator == (const FPlane& p) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p1 = XMLoadFloat4(this);
	const XMVECTOR p2 = XMLoadFloat4(&p);
	return XMPlaneEqual(p1, p2);
}

bool FPlane::operator != (const FPlane& p) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p1 = XMLoadFloat4(this);
	const XMVECTOR p2 = XMLoadFloat4(&p);
	return XMPlaneNotEqual(p1, p2);
}

//------------------------------------------------------------------------------
// FPlane operations
//------------------------------------------------------------------------------

void FPlane::Normalize() noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(this);
	XMStoreFloat4(this, XMPlaneNormalize(p));
}

void FPlane::Normalize(FPlane& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMPlaneNormalize(p));
}

float FPlane::Dot(const FVector4& v) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(this);
	const XMVECTOR v0 = XMLoadFloat4(&v);
	return XMVectorGetX(XMPlaneDot(p, v0));
}

float FPlane::DotCoordinate(const FVector3& position) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(this);
	const XMVECTOR v0 = XMLoadFloat3(&position);
	return XMVectorGetX(XMPlaneDotCoord(p, v0));
}

float FPlane::DotNormal(const FVector3& normal) const noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(this);
	const XMVECTOR n0 = XMLoadFloat3(&normal);
	return XMVectorGetX(XMPlaneDotNormal(p, n0));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

void FPlane::JTransform(const FPlane& plane, const FMatrix& M, FPlane& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(&plane);
	const XMMATRIX m0 = XMLoadFloat4x4(&M);
	XMStoreFloat4(&result, XMPlaneTransform(p, m0));
}

FPlane FPlane::JTransform(const FPlane& plane, const FMatrix& M) noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(&plane);
	const XMMATRIX m0 = XMLoadFloat4x4(&M);

	FPlane result;
	XMStoreFloat4(&result, XMPlaneTransform(p, m0));
	return result;
}

void FPlane::JTransform(const FPlane& plane, const FQuaternion& rotation, FPlane& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(&plane);
	const XMVECTOR q = XMLoadFloat4(&rotation);
	XMVECTOR X = XMVector3Rotate(p, q);
	X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
	XMStoreFloat4(&result, X);
}

FPlane FPlane::JTransform(const FPlane& plane, const FQuaternion& rotation) noexcept
{
	using namespace DirectX;
	const XMVECTOR p = XMLoadFloat4(&plane);
	const XMVECTOR q = XMLoadFloat4(&rotation);
	XMVECTOR X = XMVector3Rotate(p, q);
	X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

	FPlane result;
	XMStoreFloat4(&result, X);
	return result;
}


/****************************************************************************
	*
	* FQuaternion
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

bool FQuaternion::operator == (const FQuaternion& q) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	return XMQuaternionEqual(q1, q2);
}

bool FQuaternion::operator != (const FQuaternion& q) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	return XMQuaternionNotEqual(q1, q2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FQuaternion& FQuaternion::operator+= (const FQuaternion& q) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	XMStoreFloat4(this, XMVectorAdd(q1, q2));
	return *this;
}

FQuaternion& FQuaternion::operator-= (const FQuaternion& q) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	XMStoreFloat4(this, XMVectorSubtract(q1, q2));
	return *this;
}

FQuaternion& FQuaternion::operator*= (const FQuaternion& q) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
	return *this;
}

FQuaternion& FQuaternion::operator*= (float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(this, XMVectorScale(q, S));
	return *this;
}

FQuaternion& FQuaternion::operator/= (const FQuaternion& q) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	XMVECTOR q2 = XMLoadFloat4(&q);
	q2 = XMQuaternionInverse(q2);
	XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

FQuaternion FQuaternion::operator- () const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);

	FQuaternion R;
	XMStoreFloat4(&R, XMVectorNegate(q));
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FQuaternion operator+ (const FQuaternion& Q1, const FQuaternion& Q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(&Q1);
	const XMVECTOR q2 = XMLoadFloat4(&Q2);

	FQuaternion R;
	XMStoreFloat4(&R, XMVectorAdd(q1, q2));
	return R;
}

FQuaternion operator- (const FQuaternion& Q1, const FQuaternion& Q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(&Q1);
	const XMVECTOR q2 = XMLoadFloat4(&Q2);

	FQuaternion R;
	XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
	return R;
}

FQuaternion operator* (const FQuaternion& Q1, const FQuaternion& Q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(&Q1);
	const XMVECTOR q2 = XMLoadFloat4(&Q2);

	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
	return R;
}

FQuaternion operator* (const FQuaternion& Q, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(&Q);

	FQuaternion R;
	XMStoreFloat4(&R, XMVectorScale(q, S));
	return R;
}

FQuaternion operator/ (const FQuaternion& Q1, const FQuaternion& Q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(&Q1);
	XMVECTOR q2 = XMLoadFloat4(&Q2);
	q2 = XMQuaternionInverse(q2);

	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
	return R;
}

FQuaternion operator* (float S, const FQuaternion& Q) noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(&Q);

	FQuaternion R;
	XMStoreFloat4(&R, XMVectorScale(q1, S));
	return R;
}

//------------------------------------------------------------------------------
// FQuaternion operations
//------------------------------------------------------------------------------

float FQuaternion::Length() const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	return XMVectorGetX(XMQuaternionLength(q));
}

float FQuaternion::LengthSquared() const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	return XMVectorGetX(XMQuaternionLengthSq(q));
}

void FQuaternion::Normalize() noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(this, XMQuaternionNormalize(q));
}

void FQuaternion::Normalize(FQuaternion& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMQuaternionNormalize(q));
}

void FQuaternion::Conjugate() noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(this, XMQuaternionConjugate(q));
}

void FQuaternion::Conjugate(FQuaternion& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMQuaternionConjugate(q));
}

void FQuaternion::Inverse(FQuaternion& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMQuaternionInverse(q));
}

float FQuaternion::Dot(const FQuaternion& q) const noexcept
{
	using namespace DirectX;
	const XMVECTOR q1 = XMLoadFloat4(this);
	const XMVECTOR q2 = XMLoadFloat4(&q);
	return XMVectorGetX(XMQuaternionDot(q1, q2));
}

void FQuaternion::RotateTowards(const FQuaternion& target, float maxAngle) noexcept
{
	RotateTowards(target, maxAngle, *this);
}

FVector3 FQuaternion::ToEuler() const noexcept
{
	const float xx = x * x;
	const float yy = y * y;
	const float zz = z * z;

	const float m31 = 2.f * x * z + 2.f * y * w;
	const float m32 = 2.f * y * z - 2.f * x * w;
	const float m33 = 1.f - 2.f * xx - 2.f * yy;

	const float cy = sqrtf(m33 * m33 + m31 * m31);
	const float cx = atan2f(-m32, cy);
	if (cy > 16.f * FLT_EPSILON)
	{
		const float m12 = 2.f * x * y + 2.f * z * w;
		const float m22 = 1.f - 2.f * xx - 2.f * zz;

		return FVector3(cx, atan2f(m31, m33), atan2f(m12, m22));
	}
	else
	{
		const float m11 = 1.f - 2.f * yy - 2.f * zz;
		const float m21 = 2.f * x * y - 2.f * z * w;

		return FVector3(cx, 0.f, atan2f(-m21, m11));
	}
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

FQuaternion FQuaternion::CreateFromAxisAngle(const FVector3& axis, float angle) noexcept
{
	using namespace DirectX;
	const XMVECTOR a = XMLoadFloat3(&axis);

	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
	return R;
}

FQuaternion FQuaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
	using namespace DirectX;
	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
	return R;
}

FQuaternion FQuaternion::CreateFromYawPitchRoll(const FVector3& angles) noexcept
{
	using namespace DirectX;
	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationRollPitchYawFromVector(angles));
	return R;
}

FQuaternion FQuaternion::CreateFromRotationMatrix(const FMatrix& M) noexcept
{
	using namespace DirectX;
	const XMMATRIX M0 = XMLoadFloat4x4(&M);

	FQuaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
	return R;
}

void FQuaternion::Lerp(const FQuaternion& q1, const FQuaternion& q2, float t, FQuaternion& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);

	const XMVECTOR dot = XMVector4Dot(Q0, Q1);

	XMVECTOR R;
	if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
	{
		R = XMVectorLerp(Q0, Q1, t);
	}
	else
	{
		const XMVECTOR tv = XMVectorReplicate(t);
		const XMVECTOR t1v = XMVectorReplicate(1.f - t);
		const XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
		const XMVECTOR X1 = XMVectorMultiply(Q1, tv);
		R = XMVectorSubtract(X0, X1);
	}

	XMStoreFloat4(&result, XMQuaternionNormalize(R));
}

FQuaternion FQuaternion::Lerp(const FQuaternion& q1, const FQuaternion& q2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);

	const XMVECTOR dot = XMVector4Dot(Q0, Q1);

	XMVECTOR R;
	if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
	{
		R = XMVectorLerp(Q0, Q1, t);
	}
	else
	{
		const XMVECTOR tv = XMVectorReplicate(t);
		const XMVECTOR t1v = XMVectorReplicate(1.f - t);
		const XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
		const XMVECTOR X1 = XMVectorMultiply(Q1, tv);
		R = XMVectorSubtract(X0, X1);
	}

	FQuaternion result;
	XMStoreFloat4(&result, XMQuaternionNormalize(R));
	return result;
}

void FQuaternion::Slerp(const FQuaternion& q1, const FQuaternion& q2, float t, FQuaternion& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);
	XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
}

FQuaternion FQuaternion::Slerp(const FQuaternion& q1, const FQuaternion& q2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);

	FQuaternion result;
	XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
	return result;
}

void FQuaternion::Concatenate(const FQuaternion& q1, const FQuaternion& q2, FQuaternion& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);
	XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
}

FQuaternion FQuaternion::Concatenate(const FQuaternion& q1, const FQuaternion& q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);

	FQuaternion result;
	XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
	return result;
}

FQuaternion FQuaternion::FromToRotation(const FVector3& fromDir, const FVector3& toDir) noexcept
{
	FQuaternion result;
	FromToRotation(fromDir, toDir, result);
	return result;
}

FQuaternion FQuaternion::LookRotation(const FVector3& forward, const FVector3& up) noexcept
{
	FQuaternion result;
	LookRotation(forward, up, result);
	return result;
}

float FQuaternion::Angle(const FQuaternion& q1, const FQuaternion& q2) noexcept
{
	using namespace DirectX;
	const XMVECTOR Q0 = XMLoadFloat4(&q1);
	const XMVECTOR Q1 = XMLoadFloat4(&q2);

	// We can use the conjugate here instead of inverse assuming q1 & q2 are normalized.
	XMVECTOR R = XMQuaternionMultiply(XMQuaternionConjugate(Q0), Q1);

	const float rs = XMVectorGetW(R);
	R = XMVector3Length(R);
	return 2.f * atan2f(XMVectorGetX(R), rs);
}


/****************************************************************************
	*
	* FColor
	*
	****************************************************************************/

FColor::FColor(const DirectX::PackedVector::XMCOLOR& Packed) noexcept
{
	using namespace DirectX;
	XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
}

FColor::FColor(const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept
{
	using namespace DirectX;
	XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------
bool FColor::operator == (const FColor& c) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	return XMColorEqual(c1, c2);
}

bool FColor::operator != (const FColor& c) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	return XMColorNotEqual(c1, c2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FColor& FColor::operator= (const DirectX::PackedVector::XMCOLOR& Packed) noexcept
{
	using namespace DirectX;
	XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
	return *this;
}

FColor& FColor::operator= (const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept
{
	using namespace DirectX;
	XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
	return *this;
}

FColor& FColor::operator+= (const FColor& c) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	XMStoreFloat4(this, XMVectorAdd(c1, c2));
	return *this;
}

FColor& FColor::operator-= (const FColor& c) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	XMStoreFloat4(this, XMVectorSubtract(c1, c2));
	return *this;
}

FColor& FColor::operator*= (const FColor& c) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	XMStoreFloat4(this, XMVectorMultiply(c1, c2));
	return *this;
}

FColor& FColor::operator*= (float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(this, XMVectorScale(c, S));
	return *this;
}

FColor& FColor::operator/= (const FColor& c) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(this);
	const XMVECTOR c2 = XMLoadFloat4(&c);
	XMStoreFloat4(this, XMVectorDivide(c1, c2));
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

FColor FColor::operator- () const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	FColor R;
	XMStoreFloat4(&R, XMVectorNegate(c));
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

FColor operator+ (const FColor& C1, const FColor& C2) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(&C1);
	const XMVECTOR c2 = XMLoadFloat4(&C2);
	FColor R;
	XMStoreFloat4(&R, XMVectorAdd(c1, c2));
	return R;
}

FColor operator- (const FColor& C1, const FColor& C2) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(&C1);
	const XMVECTOR c2 = XMLoadFloat4(&C2);
	FColor R;
	XMStoreFloat4(&R, XMVectorSubtract(c1, c2));
	return R;
}

FColor operator* (const FColor& C1, const FColor& C2) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(&C1);
	const XMVECTOR c2 = XMLoadFloat4(&C2);
	FColor R;
	XMStoreFloat4(&R, XMVectorMultiply(c1, c2));
	return R;
}

FColor operator* (const FColor& C, float S) noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(&C);
	FColor R;
	XMStoreFloat4(&R, XMVectorScale(c, S));
	return R;
}

FColor operator/ (const FColor& C1, const FColor& C2) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(&C1);
	const XMVECTOR c2 = XMLoadFloat4(&C2);
	FColor R;
	XMStoreFloat4(&R, XMVectorDivide(c1, c2));
	return R;
}

FColor operator* (float S, const FColor& C) noexcept
{
	using namespace DirectX;
	const XMVECTOR c1 = XMLoadFloat4(&C);
	FColor R;
	XMStoreFloat4(&R, XMVectorScale(c1, S));
	return R;
}

//------------------------------------------------------------------------------
// FColor operations
//------------------------------------------------------------------------------

DirectX::PackedVector::XMCOLOR FColor::BGRA() const noexcept
{
	using namespace DirectX;
	const XMVECTOR clr = XMLoadFloat4(this);
	PackedVector::XMCOLOR Packed;
	PackedVector::XMStoreColor(&Packed, clr);
	return Packed;
}

DirectX::PackedVector::XMUBYTEN4 FColor::RGBA() const noexcept
{
	using namespace DirectX;
	const XMVECTOR clr = XMLoadFloat4(this);
	PackedVector::XMUBYTEN4 Packed;
	PackedVector::XMStoreUByteN4(&Packed, clr);
	return Packed;
}

FVector3 FColor::ToVector3() const noexcept
{
	return FVector3(x, y, z);
}

FVector4 FColor::ToVector4() const noexcept
{
	return FVector4(x, y, z, w);
}

void FColor::Negate() noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(this, XMColorNegative(c));
}

void FColor::Negate(FColor& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMColorNegative(c));
}

void FColor::Saturate() noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(this, XMVectorSaturate(c));
}

void FColor::Saturate(FColor& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMVectorSaturate(c));
}

void FColor::Premultiply() noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMVECTOR a = XMVectorSplatW(c);
	a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
	XMStoreFloat4(this, XMVectorMultiply(c, a));
}

void FColor::Premultiply(FColor& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMVECTOR a = XMVectorSplatW(c);
	a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
	XMStoreFloat4(&result, XMVectorMultiply(c, a));
}

void FColor::AdjustSaturation(float sat) noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(this, XMColorAdjustSaturation(c, sat));
}

void FColor::AdjustSaturation(float sat, FColor& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMColorAdjustSaturation(c, sat));
}

void FColor::AdjustContrast(float contrast) noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(this, XMColorAdjustContrast(c, contrast));
}

void FColor::AdjustContrast(float contrast, FColor& result) const noexcept
{
	using namespace DirectX;
	const XMVECTOR c = XMLoadFloat4(this);
	XMStoreFloat4(&result, XMColorAdjustContrast(c, contrast));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

void FColor::Modulate(const FColor& c1, const FColor& c2, FColor& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR C0 = XMLoadFloat4(&c1);
	const XMVECTOR C1 = XMLoadFloat4(&c2);
	XMStoreFloat4(&result, XMColorModulate(C0, C1));
}

FColor FColor::Modulate(const FColor& c1, const FColor& c2) noexcept
{
	using namespace DirectX;
	const XMVECTOR C0 = XMLoadFloat4(&c1);
	const XMVECTOR C1 = XMLoadFloat4(&c2);

	FColor result;
	XMStoreFloat4(&result, XMColorModulate(C0, C1));
	return result;
}

void FColor::Lerp(const FColor& c1, const FColor& c2, float t, FColor& result) noexcept
{
	using namespace DirectX;
	const XMVECTOR C0 = XMLoadFloat4(&c1);
	const XMVECTOR C1 = XMLoadFloat4(&c2);
	XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
}

FColor FColor::Lerp(const FColor& c1, const FColor& c2, float t) noexcept
{
	using namespace DirectX;
	const XMVECTOR C0 = XMLoadFloat4(&c1);
	const XMVECTOR C1 = XMLoadFloat4(&c2);

	FColor result;
	XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
	return result;
}


/****************************************************************************
	*
	* FRay
	*
	****************************************************************************/

	//-----------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
bool FRay::operator == (const FRay& r) const noexcept
{
	using namespace DirectX;
	const XMVECTOR r1p = XMLoadFloat3(&position);
	const XMVECTOR r2p = XMLoadFloat3(&r.position);
	const XMVECTOR r1d = XMLoadFloat3(&direction);
	const XMVECTOR r2d = XMLoadFloat3(&r.direction);
	return XMVector3Equal(r1p, r2p) && XMVector3Equal(r1d, r2d);
}

bool FRay::operator != (const FRay& r) const noexcept
{
	using namespace DirectX;
	const XMVECTOR r1p = XMLoadFloat3(&position);
	const XMVECTOR r2p = XMLoadFloat3(&r.position);
	const XMVECTOR r1d = XMLoadFloat3(&direction);
	const XMVECTOR r2d = XMLoadFloat3(&r.direction);
	return XMVector3NotEqual(r1p, r2p) && XMVector3NotEqual(r1d, r2d);
}

//-----------------------------------------------------------------------------
// FRay operators
//------------------------------------------------------------------------------

bool FRay::Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const noexcept
{
	return sphere.Intersects(position, direction, Dist);
}

bool FRay::Intersects(const BoundingBox& box, _Out_ float& Dist) const noexcept
{
	return box.Intersects(position, direction, Dist);
}

bool FRay::Intersects(const FVector3& tri0, const FVector3& tri1, const FVector3& tri2, _Out_ float& Dist) const noexcept
{
	return DirectX::TriangleTests::Intersects(position, direction, tri0, tri1, tri2, Dist);
}

bool FRay::Intersects(const FPlane& plane, _Out_ float& Dist) const noexcept
{
	using namespace DirectX;

	const XMVECTOR p = XMLoadFloat4(&plane);
	const XMVECTOR dir = XMLoadFloat3(&direction);

	const XMVECTOR nd = XMPlaneDotNormal(p, dir);

	if (XMVector3LessOrEqual(XMVectorAbs(nd), g_RayEpsilon))
	{
		Dist = 0.f;
		return false;
	}
	else
	{
		// t = -(dot(n,origin) + D) / dot(n,dir)
		const XMVECTOR pos = XMLoadFloat3(&position);
		XMVECTOR v = XMPlaneDotNormal(p, pos);
		v = XMVectorAdd(v, XMVectorSplatW(p));
		v = XMVectorDivide(v, nd);
		float dist = -XMVectorGetX(v);
		if (dist < 0)
		{
			Dist = 0.f;
			return false;
		}
		else
		{
			Dist = dist;
			return true;
		}
	}
}


/****************************************************************************
	*
	* FViewport
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

#if (__cplusplus < 202002L)
bool FViewport::operator == (const FViewport& vp) const noexcept
{
	return (x == vp.x && y == vp.y
		&& width == vp.width && height == vp.height
		&& minDepth == vp.minDepth && maxDepth == vp.maxDepth);
}

bool FViewport::operator != (const FViewport& vp) const noexcept
{
	return (x != vp.x || y != vp.y
		|| width != vp.width || height != vp.height
		|| minDepth != vp.minDepth || maxDepth != vp.maxDepth);
}
#endif

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

FViewport& FViewport::operator= (const RECT& rct) noexcept
{
	x = float(rct.left); y = float(rct.top);
	width = float(rct.right - rct.left);
	height = float(rct.bottom - rct.top);
	minDepth = 0.f; maxDepth = 1.f;
	return *this;
}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
FViewport& FViewport::operator= (const D3D11_VIEWPORT& vp) noexcept
{
	x = vp.TopLeftX; y = vp.TopLeftY;
	width = vp.Width; height = vp.Height;
	minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
	return *this;
}
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
FViewport& FViewport::operator= (const D3D12_VIEWPORT& vp) noexcept
{
	x = vp.TopLeftX; y = vp.TopLeftY;
	width = vp.Width; height = vp.Height;
	minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
	return *this;
}
#endif

//------------------------------------------------------------------------------
// FViewport operations
//------------------------------------------------------------------------------

float FViewport::AspectRatio() const noexcept
{
	if (width == 0.f || height == 0.f)
		return 0.f;

	return (width / height);
}

FVector3 FViewport::Project(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world) const noexcept
{
	using namespace DirectX;
	XMVECTOR v = XMLoadFloat3(&p);
	const XMMATRIX projection = XMLoadFloat4x4(&proj);
	v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
	FVector3 result;
	XMStoreFloat3(&result, v);
	return result;
}

void FViewport::Project(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world, FVector3& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v = XMLoadFloat3(&p);
	const XMMATRIX projection = XMLoadFloat4x4(&proj);
	v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
	XMStoreFloat3(&result, v);
}

FVector3 FViewport::Unproject(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world) const noexcept
{
	using namespace DirectX;
	XMVECTOR v = XMLoadFloat3(&p);
	const XMMATRIX projection = XMLoadFloat4x4(&proj);
	v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
	FVector3 result;
	XMStoreFloat3(&result, v);
	return result;
}

void FViewport::Unproject(const FVector3& p, const FMatrix& proj, const FMatrix& view, const FMatrix& world, FVector3& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v = XMLoadFloat3(&p);
	const XMMATRIX projection = XMLoadFloat4x4(&proj);
	v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
	XMStoreFloat3(&result, v);
}
