#include "Graphics/RRenderTarget.h"
#include "Resource/RTexture.h"



RRenderTarget::RRenderTarget(const FRenderTargetSpecification& spec) : 
	mSpecification(spec),
	mDepthAttachmentSpecification(ERenderTragetFormat::None),
	mDepthAttachment(nullptr)
{
	for (auto spec : mSpecification.Attachments.Attachments)
	{
		if (spec.TextureFormat == ERenderTragetFormat::DEPTH24STENCIL8)
			mDepthAttachmentSpecification = spec;
		else
			mSpecifications.emplace_back(spec);
	}

	// Create render target
	Invalidate();
}

RRenderTarget::~RRenderTarget()
{
	for (RTexture* texture : mAttachments)
	{
		delete texture;
		texture = nullptr;
	}
	mAttachments.clear();

	delete mDepthAttachment;
	mDepthAttachment = nullptr;
}

RRenderTarget* RRenderTarget::Create(const FRenderTargetSpecification& spec)
{
	RRenderTarget* renderTarget = new RRenderTarget(spec);

	return renderTarget;
}

void RRenderTarget::Invalidate()
{
	// delete prev attachments
	if (mAttachments.size() > 0)
	{
		// Todo: render target attachments clear and invalidate
		for (RTexture* texture : mAttachments)
		{
			delete texture;
			texture = nullptr;
		}

		mAttachments.clear();

		delete mDepthAttachment;
		mDepthAttachment = nullptr;
	}

	// Create render target attachments
	if (mSpecifications.size() <= 0)
		assert(false && "Render target specification is empty!");

	for (auto spec : mSpecifications)
	{
		RTexture* texture = new RTexture();
		switch (spec.TextureFormat)
		{
		case ERenderTragetFormat::RGBA8:
			texture->Create(mSpecification.Width, mSpecification.Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM
				, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
			break;
		case ERenderTragetFormat::RED_INTEGER:
			texture->Create(mSpecification.Width, mSpecification.Height, DXGI_FORMAT::DXGI_FORMAT_R32_UINT
				, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
			break;
		case ERenderTragetFormat::SHADER_RESOURCE:
			texture->Create(mSpecification.Width, mSpecification.Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM
				, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
			break;

		default:
			break;
		}

		mAttachments.emplace_back(texture);
	}

	// Create depth attachment
	if (mDepthAttachmentSpecification.TextureFormat != ERenderTragetFormat::None)
	{
		mDepthAttachment = new RTexture();
		mDepthAttachment->Create(mSpecification.Width, mSpecification.Height, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT
			, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	}
}

void RRenderTarget::Bind()
{
	// Todo: render target bind
}

void RRenderTarget::Unbind()
{
	// Todo: render target unbind
}

void RRenderTarget::Resize(UINT width, UINT height)
{
	static const uint32_t MaxFramebufferSize = 8192;
	if (width == 0 || height == 0 || width > MaxFramebufferSize || height > MaxFramebufferSize)
	{
		assert(false && "Attempted to rezize framebuffer");
		return;
	}

	mSpecification.Width = width;
	mSpecification.Height = height;

	Invalidate();
}

int RRenderTarget::ReadPixel(uint32_t attachmentIndex, int x, int y)
{
	// Todo: read pixel
	return 0;
}

void RRenderTarget::ClearAttachment(UINT index, const void* value)
{
	// Todo: clear attachment
}