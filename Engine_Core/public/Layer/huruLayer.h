#pragma once

#include "Common/huruEntity.h"
#include "GameObject/huruGameObject.h"

namespace huru
{
	class Layer : public Entity
	{
	public:
		Layer();
		~Layer();

		virtual void	Initialize();
		virtual void	Update();
		virtual void	LateUpdate();
		virtual void	Render();
		virtual void	Destroy();

		void	AddGameObject(GameObject* gameObj);
		void	EraseGameObject(GameObject* eraseGameObj);

		const vector<GameObject*>	GetGameObjects()	{ return mGameObjects; }

	private:
		void	findDeadGameObjects(OUT vector<GameObject*>& gameObjs);
		void	deleteGameObjects(vector<GameObject*> gameObjs);
		void	eraseDeadGameObject();

	private:
		vector<GameObject*>		mGameObjects;
	};

	typedef vector<GameObject*>::iterator GameObjectIter;
}

