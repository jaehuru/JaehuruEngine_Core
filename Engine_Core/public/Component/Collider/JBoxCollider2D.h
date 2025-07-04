#pragma once

#include "Component/Collider/JCollider.h"


class JBoxCollider2D : public JCollider
{
public:
	JBoxCollider2D();
	~JBoxCollider2D();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

    void	Serialize(json& jsonObject) const		override;
    void	Deserialize(const json& jsonObject)		override;

private:
};