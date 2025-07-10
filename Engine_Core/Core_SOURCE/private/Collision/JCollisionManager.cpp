#include "Collision/JCollisionManager.h"
#include "Scene/JSceneManager.h"
#include "Actor/AActor.h"
#include "Component/Collider/JCollider.h"
#include "Component/Transform/JTransform.h"



bitset<(UINT)ELayerType::Max>JCollisionManager::mCollisionLayerMatrix[(UINT)ELayerType::Max] = { };
unordered_map<UINT64, bool> JCollisionManager::mCollisionMap = { };

JCollisionManager::JCollisionManager()
{

}

JCollisionManager::~JCollisionManager()
{

}

void JCollisionManager::Initialize()
{
		
}

void JCollisionManager::Update()
{
	for (UINT row = 0; row < (UINT)ELayerType::Max; row++)
	{
		for (UINT col = 0; col < (UINT)ELayerType::Max; col++)
		{
			if (mCollisionLayerMatrix[row][col] == true)
			{
				LayerCollision((ELayerType)row, (ELayerType)col);
			}
		}
	}
}

void JCollisionManager::LateUpdate()
{

}

void JCollisionManager::Render()
{

}

void JCollisionManager::Clear()
{
	mCollisionMap.clear();
	mCollisionLayerMatrix->reset();
}

void JCollisionManager::CollisionLayerCheck(ELayerType left,
											ELayerType right,
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

void JCollisionManager::LayerCollision(ELayerType left, ELayerType right)
{
	const vector<AActor*>& leftObjs = JSceneManager::GetActors(left);
	const vector<AActor*>& rightObjs = JSceneManager::GetActors(right);

	for (AActor* leftObj : leftObjs)
	{
		if (leftObj->IsActive() == false)
			continue;

		JCollider* leftCol = leftObj->GetComponent<JCollider>();
		if (leftCol == nullptr)
			continue;
			
		for (AActor* rightObj : rightObjs)
		{
			if (rightObj->IsActive() == false)
				continue;

			JCollider* rightCol = rightObj->GetComponent<JCollider>();
			if (rightCol == nullptr)
				continue;

			if (leftObj == rightObj)
				continue;

			ColliderCollision(leftCol, rightCol);
		}
	}
}

void JCollisionManager::ColliderCollision(JCollider* left, JCollider* right)
{
	// 두 충돌체 번호를 가져온 ID를 확인해서 CollisionID값을 세팅
	FCollisionID cID = {};
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

bool JCollisionManager::Intersect(JCollider* left, JCollider* right)
{
	EColliderType leftType = left->GetColliderType();
	EColliderType rightType = right->GetColliderType();

	// Rect - Rect
	if (leftType == EColliderType::Rect2D && rightType == EColliderType::Rect2D)
	{
		return IntersectRectRect(left, right);
	}
	// Circle - Circle
	else if (leftType == EColliderType::Circle2D && rightType == EColliderType::Circle2D)
	{
		return IntersectCircleCircle(left, right);
	}
	// Circle - Rect or Rect - Circle
	else if ((leftType == EColliderType::Circle2D && rightType == EColliderType::Rect2D) ||
		(leftType == EColliderType::Rect2D && rightType == EColliderType::Circle2D))
	{
		JCollider* circle = (leftType == EColliderType::Circle2D) ? left : right;
		JCollider* rect = (leftType == EColliderType::Rect2D) ? left : right;
		return IntersectCircleRect(circle, rect);
	}

	return false;
}

FVector2 JCollisionManager::CalculateCollisionDepth(JCollider* left, JCollider* right)
{
	JTransform* leftTr = left->GetOwner()->GetComponent<JTransform>();
	JTransform* rightTr = right->GetOwner()->GetComponent<JTransform>();

	FVector2 leftPos = leftTr->GetPosition() + left->GetOffset();
	FVector2 rightPos = rightTr->GetPosition() + right->GetOffset();

	FVector2 leftSize = left->GetSize() * 100.0f;
	FVector2 rightSize = right->GetSize() * 100.0f;

	// 각 축의 거리 차이
	float deltaX = rightPos.x - leftPos.x;
	float deltaY = rightPos.y - leftPos.y;

	// 겹친 깊이 계산 (절대값)
	float intersectX = (leftSize.x / 2.0f + rightSize.x / 2.0f) - fabs(deltaX);
	float intersectY = (leftSize.y / 2.0f + rightSize.y / 2.0f) - fabs(deltaY);

	// 겹치지 않으면 (음수) 0 반환
	if (intersectX < 0 || intersectY < 0)
		return FVector2(0.0f, 0.0f);

	// 깊이 벡터: x, y 각각 어느 방향으로 밀어야 하는지 방향 포함
	float depthX = (deltaX > 0) ? -intersectX : intersectX;
	float depthY = (deltaY > 0) ? -intersectY : intersectY;

	return FVector2(depthX, depthY);
}

bool JCollisionManager::CheckCollisionWithDepth(JCollider* left, JCollider* right, FVector2& outDepth)
{
	if (Intersect(left, right))
	{
		outDepth = FVector2(0.0f, 0.0f);
		if (left->GetColliderType() == EColliderType::Rect2D
			&& right->GetColliderType() == EColliderType::Rect2D)
		{
			outDepth = CalculateCollisionDepth(left, right);
		}
		return true;
	}
	outDepth = FVector2(0.0f, 0.0f);
	return false;
}

bool JCollisionManager::IntersectRectRect(JCollider* left, JCollider* right)
{
	JTransform* leftTr = left->GetOwner()->GetComponent<JTransform>();
	JTransform* rightTr = right->GetOwner()->GetComponent<JTransform>();

	FVector2 leftPos = leftTr->GetPosition() + left->GetOffset();
	FVector2 rightPos = rightTr->GetPosition() + right->GetOffset();

	FVector2 leftSize = left->GetSize() * 100.0f;
	FVector2 rightSize = right->GetSize() * 100.0f;

	return (fabs(leftPos.x - rightPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f) &&
		fabs(leftPos.y - rightPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f));
}

bool JCollisionManager::IntersectCircleCircle(JCollider* left, JCollider* right)
{
	JTransform* leftTr = left->GetOwner()->GetComponent<JTransform>();
	JTransform* rightTr = right->GetOwner()->GetComponent<JTransform>();

	FVector2 leftPos = leftTr->GetPosition() + left->GetOffset();
	FVector2 rightPos = rightTr->GetPosition() + right->GetOffset();

	FVector2 leftSize = left->GetSize() * 100.0f;
	FVector2 rightSize = right->GetSize() * 100.0f;

	FVector2 leftCirclePos = leftPos + (leftSize / 2.0f);
	FVector2 rightCirclePos = rightPos + (rightSize / 2.0f);

	float distance = (leftCirclePos - rightCirclePos).Length();

	return (distance <= (leftSize.x / 2.0f + rightSize.x / 2.0f));
}

bool JCollisionManager::IntersectCircleRect(JCollider* circle, JCollider* rect)
{
	JTransform* circleTr = circle->GetOwner()->GetComponent<JTransform>();
	JTransform* rectTr = rect->GetOwner()->GetComponent<JTransform>();

	FVector2 circlePos = circleTr->GetPosition() + circle->GetOffset();
	FVector2 circleSize = circle->GetSize() * 100.0f;
	FVector2 circleCenter = circlePos + (circleSize / 2.0f);
	float circleRadius = circleSize.x / 2.0f;

	FVector2 rectPos = rectTr->GetPosition() + rect->GetOffset();
	FVector2 rectSize = rect->GetSize() * 100.0f;
	FVector2 rectHalf = rectSize / 2.0f;
	FVector2 rectCenter = rectPos + rectHalf;

	// 사각형 내부에서 가장 가까운 점 계산
	FVector2 closestPoint;
	closestPoint.x = max(rectCenter.x - rectHalf.x, min(circleCenter.x, rectCenter.x + rectHalf.x));
	closestPoint.y = max(rectCenter.y - rectHalf.y, min(circleCenter.y, rectCenter.y + rectHalf.y));

	// 거리 비교
	float distance = (circleCenter - closestPoint).Length();

	return (distance <= circleRadius);
}

