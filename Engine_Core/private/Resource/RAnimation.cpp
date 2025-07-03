#include "Resource/RAnimation.h"
#include "Helpers/Time.h"
#include "Component/Transform/JTransform.h"
#include "Actor/AActor.h"
#include "Component/Animator/JAnimator.h"
#include "Renderer/RRenderer.h"
#include "Component/Camera/JCamera.h"


RAnimation::RAnimation() :
	RResource(EResourceType::RAnimation),
	mAnimator(nullptr),
	mTexture(nullptr),
	mAnimationSheet{ },
	mIndex(-1),
	mTime(0.f),
	mbComplete(false)
{

}

RAnimation::~RAnimation()
{

}

HRESULT RAnimation::Save(const wstring& path)
{
	return E_NOTIMPL;
}

HRESULT RAnimation::Load(const wstring& path)
{
	return E_NOTIMPL;
}

void RAnimation::Update()
{
	if (mIndex < 0 || mIndex >= (int)mAnimationSheet.size())
		return;

	if (mAnimationSheet.empty())
		return;

	mTime += Time::DeltaTime();

	if (mAnimationSheet[mIndex].duration < mTime)
	{
		mTime = 0.f;
		if (mIndex < (int)mAnimationSheet.size() - 1)
			mIndex++;
		else
			mbComplete = true;
	}
}

void RAnimation::Render()
{
	if (!mTexture)
		return;
}

void RAnimation::CreateAnimation(const wstring& name,
								RTexture* spriteSheet,
								FVector2 leftTop, FVector2 size,
								FVector2 offset, UINT spriteLength,
								float duration)
{
	SetName(name);
	mTexture = spriteSheet;
	for (size_t i = 0; i < spriteLength; i++)
	{
		Sprite sprite = { };
		sprite.leftTop.x = leftTop.x + size.x * i;
		sprite.leftTop.y = leftTop.y;
		sprite.size = size;
		sprite.offset = offset;
		sprite.duration = duration;

		mAnimationSheet.push_back(sprite);
	}
}

void RAnimation::Reset()
{
	mIndex = 0;
	mTime = 0.f;
	mbComplete = false;
}
