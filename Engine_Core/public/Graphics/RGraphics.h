#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0

struct FVertex
{
	FVector3 pos;
	FVector4 color;
	FVector2 uv;
};

enum class EShaderStage
{
	VS,		// FVertex RShader
	HS,		// Hull RShader
	DS,		// Domain RShader
	GS,		// Geometry RShader
	PS,		// Pixel RShader
	CS,		// Compute RShader
	All,
	End,
};

enum class ECBType
{
	JTransform,
	None,
	End,
};

enum class ESamplerType
{
	Point,
	Linear,
	Anisotropic,
	PostProcess,
	End,
};

enum class ERenderingMode
{
	Opaque,
	CutOut,
	Transparent,
	PostProcess,
	End,
};

enum class ETextureType
{
	Albedo,
	Normal,
	Specular,
	Smoothness,
	Metallic,
	Sprite,
	End,
};

enum class ERasterizerState
{
	SolidBack,
	SolidFront,
	SolidNone,
	WireFrame,
	End,
};

enum class EBlendState
{
	AlphaBlend,
	OneOne,
	End,
};

enum class EDepthStencilState
{
	DepthNone,
	LessEqual,
	End,
};

struct FGpuBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
	D3D11_BUFFER_DESC desc						= {};

	FGpuBuffer()				= default;
	virtual ~FGpuBuffer()		= default;
};

CBUFFER(JTransformCB, CBSLOT_TRANSFORM)
{
	FMatrix world;
	FMatrix view;
	FMatrix projection;
};
