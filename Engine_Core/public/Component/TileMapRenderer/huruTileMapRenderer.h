#pragma once

#include "Component/huruComponent.h"


namespace huru
{
	namespace graphics
	{
		class Texture;
	}

	class TileMapRenderer : public Component
	{
	public:
		TileMapRenderer();
		~TileMapRenderer();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

		Vector2		GetIndex() const				{ return mIndex; }

		void		SetTexture(Texture* texture)	{ mTexture = texture; }
		void		SetSize(const Vector2& size)	{ mSize = size; }
		void		SetScale(const Vector2& scale)	{ mScale = scale; }
		void		SetIndex(Vector2 index)			{ mIndex = index; }


	public:
		static Vector2	SelectedIndex;

	private:
		Texture*		mTexture;

		Vector2			mSize;
		Vector2			mScale;
		Vector2			mIndex;
	};
}