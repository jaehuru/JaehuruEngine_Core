#pragma once

#include "Component/JComponent.h"


class JCollider;

class JScript : public JComponent
{
public:
	JScript();
	~JScript();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

	virtual void	OnCollisionEnter(JCollider* other);
	virtual void	OnCollisionStay(JCollider* other);
	virtual void	OnCollisionExit(JCollider* other);

private:

};


