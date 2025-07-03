#pragma once

#include "Component/JComponent.h"



class RTexture;

class JTileMapRenderer : public JComponent
{
public:
	JTileMapRenderer();
	~JTileMapRenderer();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

	FVector2		GetIndex() const				{ return mIndex; }

	void		SetTexture(RTexture* texture)	{ mTexture = texture; }
	void		SetSize(const FVector2& size)	{ mSize = size; }
	void		SetScale(const FVector2& scale)	{ mScale = scale; }
	void		SetIndex(FVector2 index)			{ mIndex = index; }


public:
	static FVector2	SelectedIndex;

private:
	RTexture*		mTexture;

	FVector2			mSize;
	FVector2			mScale;
	FVector2			mIndex;
};
