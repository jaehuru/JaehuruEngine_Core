#pragma once

#include "Common/CommonInclude.h"


namespace huru
{
	class Collider;

	union CollisionID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};

		UINT64 ID;
	};

	class CollisionManager
	{
	public:
		CollisionManager();
		~CollisionManager();

		static void		Initialize();
		static void		Update();
		static void		LateUpdate();
		static void		Render();
		static void		Clear();

		static void		CollisionLayerCheck(eLayerType left,
													eLayerType right,
													bool enable);
		static void		LayerCollision(eLayerType left, eLayerType right);
		static void		ColliderCollision(Collider* left, Collider* right);
		static bool		Intersect(Collider* left, Collider* right);

		static Vector2	CalculateCollisionDepth(Collider* left, Collider* right);
		static bool		CheckCollisionWithDepth(Collider* left, Collider* right, Vector2& outDepth);

		static bool		IntersectRectRect(Collider* left, Collider* right);
		static bool		IntersectCircleCircle(Collider* left, Collider* right);
		static bool		IntersectCircleRect(Collider* circle, Collider* rect);

	private:
		static bitset<(UINT)eLayerType::Max>mCollisionLayerMatrix[(UINT)eLayerType::Max];
		static unordered_map<UINT64, bool> mCollisionMap;
	};
}

