#pragma once

#include "Resource/RResource.h"
#include "Resource/RTexture.h"


class RAnimation : public RResource
{
public:
	struct Sprite
	{
		FVector2		leftTop;
		FVector2		size;
		FVector2		offset;
		float		duration;

		Sprite() :
			leftTop(FVector2::Zero),
			size(FVector2::Zero),
			offset(FVector2::Zero),
			duration(0.f)
		{

		}
	};

	RAnimation();
	~RAnimation();

	virtual	HRESULT	Save(const wstring& path) override;
	virtual	HRESULT	Load(const wstring& path) override;

	void		Update();
	void		Render();

	void		CreateAnimation(const wstring& name,
								RTexture* spriteSheet,
								FVector2 leftTop,
								FVector2 size,
								FVector2 offset,
								UINT spriteLength,
								float duration);

	void		Reset();

	bool		IsComplete() const						{ return mbComplete; } 

	void		SetAnimator(class JAnimator* animator)	{ mAnimator = animator; }


private:
	class JAnimator*				mAnimator;
	RTexture*					mTexture;  
	vector<Sprite>				mAnimationSheet; 

	int							mIndex;
	float						mTime;
	bool						mbComplete;
};


