#pragma once

#include "Common/FEntity.h"
#include "Actor/AActor.h"


class JLayer : public FEntity
{
public:
	JLayer();
	~JLayer();

	virtual void	Initialize();
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();
	virtual void	Destroy();

	void	AddGameObject(AActor* actor);
	void	EraseGameObject(AActor* eraseActor);

	const vector<AActor*>	GetGameObjects()	{ return mActors; }

private:
	void	findDeadGameObjects(OUT vector<AActor*>& actors);
	void	deleteGameObjects(vector<AActor*> actors);
	void	eraseDeadGameObject();

private:
	vector<AActor*>		mActors;
};

typedef vector<AActor*>::iterator GameObjectIter;


