#include "Precompiled.h"
#include "CollisionShape.h"

using namespace SpringEngine;
using namespace SpringEngine::Physics;
using namespace SpringEngine::Graphics;

CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:Must call terminate");
}

void CollisionShape::InitializeEmpty()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:is already initialized");
	mCollisionShape = new btEmptyShape();
}

void CollisionShape::InitializeBox(const SpringEngine::Math::Vector3& halfExtents)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:is already initialized");
	mCollisionShape = new btBoxShape(ConvertTobtVector3(halfExtents));
}

void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:is already initialized");
	mCollisionShape = new btSphereShape(radius);
}

void CollisionShape::InitializeHull(const SpringEngine::Math::Vector3& halfExtents, const SpringEngine::Math::Vector3& origin)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape:is already initialized");
	btConvexHullShape* hullShape = new btConvexHullShape();
	std::vector<Math::Vector3> points =
	{
		{-halfExtents.x, -halfExtents.y, -halfExtents.z},
		{-halfExtents.x,  halfExtents.y, -halfExtents.z},
		{ halfExtents.x,  halfExtents.y, -halfExtents.z},
		{ halfExtents.x, -halfExtents.y, -halfExtents.z},
		{-halfExtents.x, -halfExtents.y,  halfExtents.z},
		{-halfExtents.x,  halfExtents.y,  halfExtents.z},
		{ halfExtents.x,  halfExtents.y,  halfExtents.z},
		{ halfExtents.x, -halfExtents.y,  halfExtents.z},
	};

	for (const Math::Vector3& point : points)
	{
		hullShape->addPoint(ConvertTobtVector3(point + origin), false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;
}

void CollisionShape::Terminate()
{
	SafeDelete(mCollisionShape);
}
