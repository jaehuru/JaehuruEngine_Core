#pragma once

#include "Common/CommonInclude.h"
#include "Component/huruComponent.h"


namespace huru::object
{
	void Destroy(GameObject* gameObject);
}

namespace huru
{
	class GameObject : public Entity
	{
	public:
		friend void object::Destroy(GameObject* obj);

		enum class eState
		{
			Active,
			Paused,
			Dead,
			End
		};

		GameObject();
		~GameObject();

		virtual void	Initialize();
		virtual void	Update();
		virtual void	LateUpdate();
		virtual void	Render();

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();
			comp->SetOwner(this);
			comp->Initialize();

			mComponents[(UINT)comp->GetType()] = comp;
			
			return comp;
		}

		template <typename T>
		T* GetComponent()
		{
			T* component = nullptr;
			for (Component* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (component)
					break;
			}
			return component;
		}

		eState						GetState() const		{ return mState; }
		eLayerType					GetLayerType() const	{ return mLayerType; }

		void SetActive(bool power)
		{
			if (power == true)
				mState = eState::Active;
			if (power == false)
				mState = eState::Paused;
		}
		void SetLayerType(eLayerType layerType) { mLayerType = layerType; }

		bool	IsActive() const	{ return mState == eState::Active; }
		bool	IsDead() const		{ return mState == eState::Dead; }

	private:
		void	initializeTransform();
		void	death() { mState = eState::Dead; }

	private:
		eState						mState;
		vector<Component*>			mComponents;
		eLayerType					mLayerType;
	};
}

