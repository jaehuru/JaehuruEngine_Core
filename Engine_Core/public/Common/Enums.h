#pragma once


enum class EComponentType
{
	JTransform,
	JTileMapRenderer,
	JCollider,
	JRigidbody,
	JScript,
	JSpriteRenderer,
	JAnimator,
	JCamera,
	JAudioListener,
	JAudioSource,
	End,
};

enum class ELayerType
{
	None,
	BackGround,
	Player,
	Floor,
	Particle,
	Max = 16,
};

enum class EResourceType
{
	RTexture,
	RMesh,
	RMaterial,
	RAnimation,
	RAudioClip,
	RShader,
	Prefab,
	End,
};

enum class EColliderType
{
	Circle2D,
	Rect2D,
	End,
};

enum class EUIType
{
	Button,
	HUD,
	End,
};

template<typename T>
constexpr ELayerType ToEngineLayerType(T layerEnum)
{
	return static_cast<ELayerType>(layerEnum);
}

template<typename T>
constexpr EUIType ToEngineUIType(T uiEnum)
{
	return static_cast<EUIType>(uiEnum);
}
