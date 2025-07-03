#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


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
