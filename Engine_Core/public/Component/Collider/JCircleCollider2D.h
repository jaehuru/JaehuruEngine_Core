#pragma once

#include "Component/Collider/JCollider.h"


class JCircleCollider2D : public JCollider
{
public:
	JCircleCollider2D();
	~JCircleCollider2D();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

private:
	FVector2 mRadius;
};


