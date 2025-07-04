#pragma once

#include "Common/CommonInclude.h"
#include "Component/JComponent.h"



void Destroy(AActor* actor);

class AActor : public FEntity
{
public:
	friend void Destroy(AActor* actor);

	enum class EState
	{
		Active,
		Paused,
		Dead,
		End
	};

	AActor();
	~AActor();

	virtual void	Initialize();
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();

    void	Serialize(json& jsonObject) const override;
    void	Deserialize(const json& jsonObject) override;

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
		for (JComponent* comp : mComponents)
		{
			component = dynamic_cast<T*>(comp);
			if (component)
				break;
		}
		return component;
	}

	const vector<JComponent*>&	GetComponents() const	{ return mComponents; }
	EState						GetState() const		{ return mState; }
	ELayerType					GetLayerType() const	{ return mLayerType; }

	void	SetActive(bool power)
	{
		if (power == true)
			mState = EState::Active;
		if (power == false)
			mState = EState::Paused;
	}
	void	SetLayerType(ELayerType layerType)	{ mLayerType = layerType; }

	bool	IsActive() const	{ return mState == EState::Active; }
	bool	IsDead() const		{ return mState == EState::Dead; }

private:
	void	initializeTransform();
	void	death() { mState = EState::Dead; }

private:
	EState						mState;
	vector<JComponent*>			mComponents;
	ELayerType					mLayerType;
};