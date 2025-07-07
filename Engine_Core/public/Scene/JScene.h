#pragma once

#include "Common/FEntity.h"


class AActor;
class JLayer;

class JScene : public FEntity
{
public:
	JScene();
	~JScene();

	virtual void	Initialize();
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();
	virtual void	Destroy();

	virtual void	OnEnter();
	virtual void	OnExit();

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	AddActor(AActor* actor, const ELayerType type);
	void	EraseActor(AActor* actor);

	JLayer*	GetLayer(const ELayerType type) const	{ return mLayers[(UINT)type]; }

private:
	void	createLayers();
		
private:
	vector<JLayer*>		mLayers;
};