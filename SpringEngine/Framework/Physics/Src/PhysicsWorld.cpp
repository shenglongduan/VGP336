#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace SpringEngine;
using namespace SpringEngine::Physics;

namespace 
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void Physics::PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "physicsWorld is already initialized!");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void Physics::PhysicsWorld::StaticTerminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* SpringEngine::Physics::PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "physicsWorld : is not initialized!");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: Terminate must be called!");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mInterface = new btDbvtBroadphase();
	msolver = new btSequentialImpulseConstraintSolver();
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, msolver, mCollisionConfiguration);
	mDynamicsWorld->setGravity(ConvertTobtVector3(settings.gravity));
	mDynamicsWorld->setDebugDrawer(&mPhysicsDebugDraw);

	//mSoftBodyWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, msolver, mCollisionConfiguration);
	//mSoftBodyWorld->setGravity(ConvertTobtVector3(settings.gravity));
	//mSoftBodyWorld->setDebugDrawer(&mPhysicsDebugDraw);
}

void PhysicsWorld::Terminate()
{
	//SafeDelete(mSoftBodyWorld);
	SafeDelete(mDynamicsWorld);
	SafeDelete(msolver);
	SafeDelete(mInterface);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
	mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	//mSoftBodyWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->Update();
	}
}

void PhysicsWorld::DebugUI()
{
	//later
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("DebugDraw", &mDebugDraw);
		if (mDebugDraw)
		{
			int debugMode = mPhysicsDebugDraw.getDebugMode();
			bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe);
			if (ImGui::Checkbox("[DBG]Wireframe", &isEnabled))
			{
				debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
			}

			isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb);
			if (ImGui::Checkbox("[DBG]DrawAABB", &isEnabled))
			{
				debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
			}

			mPhysicsDebugDraw.setDebugMode(debugMode);
			mDynamicsWorld->debugDrawWorld();
			//mSoftBodyWorld->debugDrawWorld();
		}
	}
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
		if (physicsObject->GetSoftBody() != nullptr)
		{
			//mSoftBodyWorld->addSoftBody(physicsObject->GetSoftBody());
		}
		else if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
		}
	}
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter != mPhysicsObjects.end())
	{
		if (physicsObject->GetSoftBody() != nullptr)
		{
			//mSoftBodyWorld->removeSoftBody(physicsObject->GetSoftBody());
		}
		else if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity)
{
	ASSERT(mDynamicsWorld != nullptr, "PhysicsWorld : static initialized was not called");
	mSettings.gravity = gravity;
	mDynamicsWorld->setGravity(ConvertTobtVector3(gravity));
}

void PhysicsWorld::SetSimSteps(uint32_t steps)
{
	mSettings.simulationSteps = steps;
}

void PhysicsWorld::SetFixedTimeStep(float timeStep)
{
	mSettings.fixedTimeStep = timeStep;
}

btSoftBody* PhysicsWorld::CreateSoftBody(int nodeCount)
{
	btSoftBody* softBody = new btSoftBody(&mSoftBodyWorld->getWorldInfo(), nodeCount, nullptr, nullptr);
	return softBody;
}
