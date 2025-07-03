#pragma once

#include "Component/huruComponent.h"


namespace huru
{
	namespace graphics
	{
		class Texture;
	}
	class Material;
	class Mesh;

	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

		void	SetSprite(Texture* sprite)				{ mSprite = sprite; }
		void	SetMaterial(Material* material)			{ mMaterial = material; }

	private:
		Texture*		mSprite;
		Material*		mMaterial;
		Mesh*			mMesh;
	}; 
}