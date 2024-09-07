#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace SpringEngine;
using namespace SpringEngine::Physics;

void PhysicsService::Update(float deltaTime)
{
	if (mEnabled)
	{
		PhysicsWorld::Get()->Update(deltaTime);
	}
}

void PhysicsService::DebugUI()
{
	if (mEnabled)
	{
		PhysicsWorld::Get()->DebugUI();
	}
}

void PhysicsService::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Gravity"))
	{
		const auto& gravity = value["Gravity"].GetArray();
		const float x = gravity[0].GetFloat();
		const float y = gravity[1].GetFloat();
		const float z = gravity[2].GetFloat();
		PhysicsWorld::Get()->SetGravity({ x,y,z });
	}
	if (value.HasMember("SimSteps"))
	{
		const uint32_t simSteps = static_cast<uint32_t>(value["SimSteps"].GetInt());
		PhysicsWorld::Get()->SetSimSteps(simSteps);
	}
	if (value.HasMember("FixedTimeStep"))
	{
		const float fixedTimeStep = value["FixedTimeStep"].GetFloat();
		PhysicsWorld::Get()->SetFixedTimeStep(fixedTimeStep);
	}
}

void PhysicsService::Register(RigidBodyComponent* rigidBodyComponent)
{
	PhysicsWorld::Get()->Register(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::Unregister(RigidBodyComponent* rigidBodyComponent)
{
	PhysicsWorld::Get()->Unregister(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::SetEnabled(bool enabled)
{
	mEnabled = enabled;
}
