#pragma once

#include "Resource/huruResource.h"
#include "Resource/huruTexture.h"

namespace huru
{
	using namespace graphics;

	class Animation : public Resource
	{
	public:
		struct Sprite
		{
			Vector2		leftTop;
			Vector2		size;
			Vector2		offset;
			float		duration;

			Sprite() :
				leftTop(Vector2::Zero),
				size(Vector2::Zero),
				offset(Vector2::Zero),
				duration(0.f)
			{

			}
		};

		Animation();
		~Animation();

		virtual	HRESULT	Save(const wstring& path) override;
		virtual	HRESULT	Load(const wstring& path) override;

		void		Update();
		void		Render();

		void		CreateAnimation(const wstring& name,
									Texture* spriteSheet,
									Vector2 leftTop,
									Vector2 size,
									Vector2 offset,
									UINT spriteLength,
									float duration);

		void		Reset();

		bool		IsComplete() const						{ return mbComplete; } 

		void		SetAnimator(class Animator* animator)	{ mAnimator = animator; }


	private:
		class Animator*				mAnimator;
		Texture*					mTexture;  
		vector<Sprite>				mAnimationSheet; 

		int							mIndex;
		float						mTime;
		bool						mbComplete;
	};
}

