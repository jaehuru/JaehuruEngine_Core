#pragma once

#include "Component/JComponent.h"


class JAudioListener : public JComponent
{
public:
	JAudioListener();
	~JAudioListener();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;
};