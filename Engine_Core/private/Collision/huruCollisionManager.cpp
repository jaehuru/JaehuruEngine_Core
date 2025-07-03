#include "Collision/huruCollisionManager.h"
#include "Scene/huruSceneManager.h"
#include "GameObject/huruGameObject.h"
#include "Component/Collider/huruCollider.h"
#include "Component/Transform/huruTransform.h"

namespace huru
{
	bitset<(UINT)eLayerType::Max>CollisionManager::mCollisionLayerMatrix[(UINT)eLayerType::Max] = { };
	unordered_map<UINT64, bool> CollisionManager::mCollisionMap = { };

	CollisionManager::CollisionManager()
	{

	}

	CollisionManager::~CollisionManager()
	{

	}

	void CollisionManager::Initialize()
	{
		
	}

	void CollisionManager::Update()
	{
		for (UINT row = 0; row < (UINT)eLayerType::Max; row++)
		{
			for (UINT col = 0; col < (UINT)eLayerType::Max; col++)
			{
				if (mCollisionLayerMatrix[row][col] == true)
				{
					LayerCollision((eLayerType)row, (eLayerType)col);
				}
			}
		}
	}

	void CollisionManager::LateUpdate()
	{

	}

	void CollisionManager::Render()
	{

	}

	void CollisionManager::Clear()
	{
		mCollisionMap.clear();
		mCollisionLayerMatrix->reset();
	}

	void CollisionManager::CollisionLayerCheck(eLayerType left,
												eLayerType right,
												bool enable)
	{
		int row = 0;
		int col = 0;

		if (left <= right)
		{
			row = (UINT)left;
			col = (UINT)right;
		}		  
		else	  
		{		 
			row = (UINT)right;
			col = (UINT)left;
		}

		mCollisionLayerMatrix[row][col] = enable;
	}

	void CollisionManager::LayerCollision(eLayerType left, eLayerType right)
	{
		const vector<GameObject*>& leftObjs = SceneManager::GetGameObjects(left);
		const vector<GameObject*>& rightObjs = SceneManager::GetGameObjects(right);

		for (GameObject* leftObj : leftObjs)
		{
			if (leftObj->IsActive() == false)
				continue;

			Collider* leftCol = leftObj->GetComponent<Collider>();
			if (leftCol == nullptr)
				continue;
			
			for (GameObject* rightObj : rightObjs)
			{
				if (rightObj->IsActive() == false)
					continue;

				Collider* rightCol = rightObj->GetComponent<Collider>();
				if (rightCol == nullptr)
					continue;

				if (leftObj == rightObj)
					continue;

				ColliderCollision(leftCol, rightCol);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		// 두 충돌체 번호를 가져온 ID를 확인해서 CollisionID값을 세팅
		CollisionID cID = {};
		cID.left = left->GetID();
		cID.right = right->GetID();

		// 해당 id로 충돌체 정보를 검색
		// 만약 충돌체 정보가 없다면 충돌정보를 생성
		auto iter = mCollisionMap.find(cID.ID);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(make_pair(cID.ID, false));
			iter = mCollisionMap.find(cID.ID);
		}

		// 충돌 체크
		if (Intersect(left, right))
		{
			// 최초 충돌
			if (iter->second == false)
			{
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				iter->second = true;
			}
			else // 충돌 지속 중
			{
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			if (iter->second == true)
			{
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		eColliderType leftType = left->GetColliderType();
		eColliderType rightType = right->GetColliderType();

		// Rect - Rect
		if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D)
		{
			return IntersectRectRect(left, right);
		}
		// Circle - Circle
		else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D)
		{
			return IntersectCircleCircle(left, right);
		}
		// Circle - Rect or Rect - Circle
		else if ((leftType == eColliderType::Circle2D && rightType == eColliderType::Rect2D) ||
			(leftType == eColliderType::Rect2D && rightType == eColliderType::Circle2D))
		{
			Collider* circle = (leftType == eColliderType::Circle2D) ? left : right;
			Collider* rect = (leftType == eColliderType::Rect2D) ? left : right;
			return IntersectCircleRect(circle, rect);
		}

		return false;
	}

	Vector2 CollisionManager::CalculateCollisionDepth(Collider* left, Collider* right)
	{
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Vector2 leftPos = leftTr->GetPosition() + left->GetOffset();
		Vector2 rightPos = rightTr->GetPosition() + right->GetOffset();

		Vector2 leftSize = left->GetSize() * 100.0f;
		Vector2 rightSize = right->GetSize() * 100.0f;

		// 각 축의 거리 차이
		float deltaX = rightPos.x - leftPos.x;
		float deltaY = rightPos.y - leftPos.y;

		// 겹친 깊이 계산 (절대값)
		float intersectX = (leftSize.x / 2.0f + rightSize.x / 2.0f) - fabs(deltaX);
		float intersectY = (leftSize.y / 2.0f + rightSize.y / 2.0f) - fabs(deltaY);

		// 겹치지 않으면 (음수) 0 반환
		if (intersectX < 0 || intersectY < 0)
			return Vector2(0.0f, 0.0f);

		// 깊이 벡터: x, y 각각 어느 방향으로 밀어야 하는지 방향 포함
		float depthX = (deltaX > 0) ? -intersectX : intersectX;
		float depthY = (deltaY > 0) ? -intersectY : intersectY;

		return Vector2(depthX, depthY);
	}

	bool CollisionManager::CheckCollisionWithDepth(Collider* left, Collider* right, Vector2& outDepth)
	{
		if (Intersect(left, right))
		{
			outDepth = Vector2(0.0f, 0.0f);
			if (left->GetColliderType() == eColliderType::Rect2D
				&& right->GetColliderType() == eColliderType::Rect2D)
			{
				outDepth = CalculateCollisionDepth(left, right);
			}
			return true;
		}
		outDepth = Vector2(0.0f, 0.0f);
		return false;
	}

	bool CollisionManager::IntersectRectRect(Collider* left, Collider* right)
	{
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Vector2 leftPos = leftTr->GetPosition() + left->GetOffset();
		Vector2 rightPos = rightTr->GetPosition() + right->GetOffset();

		Vector2 leftSize = left->GetSize() * 100.0f;
		Vector2 rightSize = right->GetSize() * 100.0f;

		return (fabs(leftPos.x - rightPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f) &&
			fabs(leftPos.y - rightPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f));
	}

	bool CollisionManager::IntersectCircleCircle(Collider* left, Collider* right)
	{
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Vector2 leftPos = leftTr->GetPosition() + left->GetOffset();
		Vector2 rightPos = rightTr->GetPosition() + right->GetOffset();

		Vector2 leftSize = left->GetSize() * 100.0f;
		Vector2 rightSize = right->GetSize() * 100.0f;

		Vector2 leftCirclePos = leftPos + (leftSize / 2.0f);
		Vector2 rightCirclePos = rightPos + (rightSize / 2.0f);

		float distance = (leftCirclePos - rightCirclePos).Length();

		return (distance <= (leftSize.x / 2.0f + rightSize.x / 2.0f));
	}

	bool CollisionManager::IntersectCircleRect(Collider* circle, Collider* rect)
	{
		Transform* circleTr = circle->GetOwner()->GetComponent<Transform>();
		Transform* rectTr = rect->GetOwner()->GetComponent<Transform>();

		Vector2 circlePos = circleTr->GetPosition() + circle->GetOffset();
		Vector2 circleSize = circle->GetSize() * 100.0f;
		Vector2 circleCenter = circlePos + (circleSize / 2.0f);
		float circleRadius = circleSize.x / 2.0f;

		Vector2 rectPos = rectTr->GetPosition() + rect->GetOffset();
		Vector2 rectSize = rect->GetSize() * 100.0f;
		Vector2 rectHalf = rectSize / 2.0f;
		Vector2 rectCenter = rectPos + rectHalf;

		// 사각형 내부에서 가장 가까운 점 계산
		Vector2 closestPoint;
		closestPoint.x = max(rectCenter.x - rectHalf.x, min(circleCenter.x, rectCenter.x + rectHalf.x));
		closestPoint.y = max(rectCenter.y - rectHalf.y, min(circleCenter.y, rectCenter.y + rectHalf.y));

		// 거리 비교
		float distance = (circleCenter - closestPoint).Length();

		return (distance <= circleRadius);
	}
}
