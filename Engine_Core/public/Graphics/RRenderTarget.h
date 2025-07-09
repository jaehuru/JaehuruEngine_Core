#pragma once

#include "Common/CommonInclude.h"

class RTexture;

enum class ERenderTragetFormat
{
	None = 0,

	// Color
	RGBA8,

	// RED 채널에 정수형 데이터 저장 (정수형 데이터 저장을 용도로 사용)
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Shader resource
	SHADER_RESOURCE,

	// Defaults
	Depth = DEPTH24STENCIL8
};

// 렌더타겟의 사용 목적을 지정하는 구조체
struct FRenderTargetTextureSpecification
{
	FRenderTargetTextureSpecification() = default;
	FRenderTargetTextureSpecification(ERenderTragetFormat format)
		: TextureFormat(format) {
	}

	ERenderTragetFormat TextureFormat = ERenderTragetFormat::None;
	// TODO: filtering/wrap
};

// 렌더타겟들(여러개)의 사용목적을 지정하는 구조체
struct FRenderTargetAttachmentSpecification
{
	FRenderTargetAttachmentSpecification() = default;
	FRenderTargetAttachmentSpecification(std::initializer_list<FRenderTargetTextureSpecification> attachments)
		: Attachments(attachments) {
	}

	vector<FRenderTargetTextureSpecification> Attachments;
};

struct FRenderTargetSpecification
{
	UINT Width = 0, Height = 0;
	FRenderTargetAttachmentSpecification Attachments;
	UINT Samples = 1;

	bool SwapChainTarget = false;
};

class RRenderTarget
{
public:
	RRenderTarget(const FRenderTargetSpecification& spec);
	virtual ~RRenderTarget();

	static RRenderTarget* Create(const FRenderTargetSpecification& spec);

	void	Invalidate();
	void	Bind();
	void	Unbind();

	void	Resize(UINT width, UINT height);
	int		ReadPixel(uint32_t attachmentIndex, int x, int y);

	void	ClearAttachment(UINT index, const void* value);

	RTexture* GetAttachmentTexture(UINT index) 
	{
		if (index >= mAttachments.size()) 
			assert(NULL && "Rendertarget is not exist"); 
		return mAttachments[index]; 
	}
	FRenderTargetSpecification& GetSpecification() { return mSpecification; }

private:
	FRenderTargetSpecification mSpecification;

	vector<FRenderTargetTextureSpecification> mSpecifications;
	FRenderTargetTextureSpecification mDepthAttachmentSpecification;

	vector<RTexture*> mAttachments;
	RTexture* mDepthAttachment;
};

