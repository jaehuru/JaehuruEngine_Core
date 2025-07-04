#pragma once

#include "Scene/JScene.h"


class JDontDestroyOnLoad : public JScene
{
public:
	JDontDestroyOnLoad();
	~JDontDestroyOnLoad();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	OnEnter()		override;
	void	OnExit()		override;

private:

};