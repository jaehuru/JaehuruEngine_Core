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
		float			duration;

		Sprite() :
			leftTop(FVector2::Zero),
			size(FVector2::Zero),
			offset(FVector2::Zero),
			duration(0.f)
		{

		}

        void Serialize(json& jsonObject) const
        {
            jsonObject["LeftTopX"] = leftTop.x;
            jsonObject["LeftTopY"] = leftTop.y;
            jsonObject["SizeX"] = size.x;
            jsonObject["SizeY"] = size.y;
            jsonObject["OffsetX"] = offset.x;
            jsonObject["OffsetY"] = offset.y;
            jsonObject["Duration"] = duration;
        }

        void Deserialize(const json& jsonObject)
        {
            leftTop.x = jsonObject["LeftTopX"];
            leftTop.y = jsonObject["LeftTopY"];
            size.x = jsonObject["SizeX"];
            size.y = jsonObject["SizeY"];
            offset.x = jsonObject["OffsetX"];
            offset.y = jsonObject["OffsetY"];
            duration = jsonObject["Duration"];
        }
	};

	RAnimation();
	~RAnimation();

	HRESULT	Save(const wstring& path) override;
	HRESULT	Load(const wstring& path) override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

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
	class JAnimator*			mAnimator;
	RTexture*					mTexture;  
	vector<Sprite>				mAnimationSheet; 

	int							mIndex;
	float						mTime;
	bool						mbComplete;
};


