#pragma once

#include "Component/JComponent.h"


class RTexture;
class RMaterial;
class RMesh;

class JSpriteRenderer : public JComponent
{
public:
	JSpriteRenderer();
	~JSpriteRenderer();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	SetSprite(RTexture* sprite)				{ mSprite = sprite; }
	void	SetMaterial(RMaterial* material)		{ mMaterial = material; }

private:
	RTexture*		mSprite;
	RMaterial*		mMaterial;
	RMesh*			mMesh;
}; 
