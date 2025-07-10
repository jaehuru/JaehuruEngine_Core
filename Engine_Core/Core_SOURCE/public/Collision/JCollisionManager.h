#pragma once

#include "Common/CommonInclude.h"



class JCollider;

union FCollisionID
{
	struct
	{
		UINT32 left;
		UINT32 right;
	};

	UINT64 ID;
};

class JCollisionManager
{
public:
	JCollisionManager();
	~JCollisionManager();

	static void		Initialize();
	static void		Update();
	static void		LateUpdate();
	static void		Render();
	static void		Clear();

	static void		CollisionLayerCheck(ELayerType left,
												ELayerType right,
												bool enable);
	static void		LayerCollision(ELayerType left, ELayerType right);
	static void		ColliderCollision(JCollider* left, JCollider* right);
	static bool		Intersect(JCollider* left, JCollider* right);

	static FVector2	CalculateCollisionDepth(JCollider* left, JCollider* right);
	static bool		CheckCollisionWithDepth(JCollider* left, JCollider* right, FVector2& outDepth);

	static bool		IntersectRectRect(JCollider* left, JCollider* right);
	static bool		IntersectCircleCircle(JCollider* left, JCollider* right);
	static bool		IntersectCircleRect(JCollider* circle, JCollider* rect);

private:
	static bitset<(UINT)ELayerType::Max>mCollisionLayerMatrix[(UINT)ELayerType::Max];
	static unordered_map<UINT64, bool> mCollisionMap;
};


