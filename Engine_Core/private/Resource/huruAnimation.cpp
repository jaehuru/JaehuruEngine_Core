#include "Resource/huruAnimation.h"
#include "Helpers/huruTime.h"
#include "Component/Transform/huruTransform.h"
#include "GameObject/huruGameObject.h"
#include "Component/Animator/huruAnimator.h"
#include "Renderer/huruRenderer.h"
#include "Component/Camera/huruCamera.h"

namespace huru
{

	Animation::Animation() :
		Resource(eResourceType::Animation),
		mAnimator(nullptr),
		mTexture(nullptr),
		mAnimationSheet{ },
		mIndex(-1),
		mTime(0.f),
		mbComplete(false)
	{

	}

	Animation::~Animation()
	{

	}

	HRESULT Animation::Save(const wstring& path)
	{
		return E_NOTIMPL;
	}

	HRESULT Animation::Load(const wstring& path)
	{
		return E_NOTIMPL;
	}

	void Animation::Update()
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

	void Animation::Render()
	{
		if (!mTexture)
			return;
	}

	void Animation::CreateAnimation(const wstring& name,
									Texture* spriteSheet,
									Vector2 leftTop, Vector2 size,
									Vector2 offset, UINT spriteLength,
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

	void Animation::Reset()
	{
		mIndex = 0;
		mTime = 0.f;
		mbComplete = false;
	}
}