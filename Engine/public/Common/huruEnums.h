#pragma once

namespace huru::enums
{
	enum class eComponentType
	{
		Transform,
		TileMapRenderer,
		Collider,
		Rigidbody,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
		End,
	};

	enum class eLayerType
	{
		None,
		Max = 16,
	};

	enum class eResourceType
	{
		Texture,
		Mesh,
		Material,
		Animation,
		AudioClip,
		Shader,
		Prefab,
		End,
	};

	enum class eColliderType
	{
		Circle2D,
		Rect2D,
		End,
	};

	enum class eUIType
	{
		Button,
		HUD,
		End,
	};

	template<typename T>
	constexpr eLayerType ToEngineLayerType(T layerEnum)
	{
		return static_cast<eLayerType>(layerEnum);
	}

	template<typename T>
	constexpr eUIType ToEngineUIType(T uiEnum)
	{
		return static_cast<eUIType>(uiEnum);
	}
}