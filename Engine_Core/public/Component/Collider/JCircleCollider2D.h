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

    void	Serialize(json& jsonObject) const		override;
    void	Deserialize(const json& jsonObject)		override;

private:
	FVector2 mRadius;
};