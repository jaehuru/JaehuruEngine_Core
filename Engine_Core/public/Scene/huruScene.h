#pragma once

#include "Common/huruEntity.h"
#include "GameObject/huruGameObject.h"
#include "Layer/huruLayer.h"

namespace huru
{
	class Scene : public Entity
	{
	public:
		Scene();
		~Scene();

		virtual void	Initialize();
		virtual void	Update();
		virtual void	LateUpdate();
		virtual void	Render();
		virtual void	Destroy();

		virtual void	OnEnter();
		virtual void	OnExit();

		void	AddGameObject(GameObject* gameObj, const eLayerType type);
		void	EraseGameObject(GameObject* gameObj);

		Layer*	GetLayer(const eLayerType type) const	{ return mLayers[(UINT)type]; }

	private:
		void	createLayers();
		
	private:
		vector<Layer*>		mLayers;
	};
}

