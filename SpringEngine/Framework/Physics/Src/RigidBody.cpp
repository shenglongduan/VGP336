#include "Precompiled.h"
#include "RigidBody.h"
#include "PhysicsWorld.h"
#include "CollisionShape.h"

#include <Graphics/Inc/Transform.h>

using namespace SpringEngine;
using namespace SpringEngine::Physics;
using namespace SpringEngine::Graphics;

RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr, "RigidBody: Terminate must be called!");
}

void RigidBody::Initialize(SpringEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
	mGraphicsTransfrom = &graphicsTransform;
	mMass = mass;

	mMotionstate = new btDefaultMotionState(ConvertTobyTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mMass, mMotionstate, shape.GetCollisionShape());
	//PhysicsWorld::Get()->Register(this);
}

void RigidBody::Terminate()
{
	//PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mRigidBody);
	SafeDelete(mMotionstate);
}

void RigidBody::SetPosition(const SpringEngine::Math::Vector3& position)
{
	if (IsDynamic())
	{
		mRigidBody->activate();
	}
	mGraphicsTransfrom->position = position;
	mRigidBody->setWorldTransform(ConvertTobyTransform(*mGraphicsTransfrom));
}

void RigidBody::SetVelocity(const SpringEngine::Math::Vector3& velocity)
{
	mRigidBody->activate();
	mRigidBody->setLinearVelocity(ConvertTobtVector3(velocity));
}

bool RigidBody::IsDynamic() const
{
	return mMass > 0.0f;
}

void RigidBody::Update()
{
	btTransform& worldTransform = mRigidBody->getWorldTransform();
	mGraphicsTransfrom->position = ConvertTobtVector3(worldTransform.getOrigin());
	mGraphicsTransfrom->rotation = ConvertTobtQuaternion(worldTransform.getRotation());
}

btRigidBody* RigidBody::GetRigidBody()
{
	return mRigidBody;
}
