#pragma once

#include "Common/IEntity.h"

class AActor;

class JLayer : public IEntity
{
public:
	JLayer();
	~JLayer();

	virtual void	Initialize();
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();
	virtual void	Destroy();

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	AddActor(AActor* actor);
	void	EraseActor(AActor* eraseActor);

	const vector<AActor*>	GetActors()	{ return mActors; }

private:
	void	findDeadActors(OUT vector<AActor*>& actors);
	void	deleteActors(vector<AActor*> actors);
	void	eraseDeadActor();

private:
	vector<AActor*>		mActors;
};

typedef vector<AActor*>::iterator ActorIter;