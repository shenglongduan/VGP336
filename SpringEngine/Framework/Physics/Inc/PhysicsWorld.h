#pragma once

#include "PhysicsDebugDraw.h"

namespace SpringEngine::Physics
{
	class PhysicsObject;

	class PhysicsWorld final
	{
	public:
		struct  Settings
		{
			SpringEngine::Math::Vector3 gravity = { 0.0f,-9.81f,0.0f };
			uint32_t simulationSteps = 10;
			float fixedTimeStep = 1.0f / 60.f;
		};

		static void StaticInitialize(const Settings& settings);
		static void StaticTerminate();
		static PhysicsWorld* Get();

		PhysicsWorld() = default;
		~PhysicsWorld();

		void Initialize(const Settings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void Register(PhysicsObject* physicsObject);
		void Unregister(PhysicsObject* physicsObject);

		void SetGravity(const Math::Vector3& gravity);
		void SetSimSteps(uint32_t steps);
		void SetFixedTimeStep(float timeStep);

	private:
		Settings mSettings;

		//bullet Objects
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btDiscreteDynamicsWorld* mDynamicsWorld = nullptr;
		btSequentialImpulseConstraintSolver* msolver = nullptr;

		using PhysicsObjects = std::vector<PhysicsObject*>;
		PhysicsObjects mPhysicsObjects;

		PhysicsDebugDraw mPhysicsDebugDraw;
		bool mDebugDraw = false;

		// soft body physics
		btSoftRigidDynamicsWorld* mSoftBodyWorld = nullptr;
		friend class SoftBody;
		btSoftBody* CreateSoftBody(int nodeCount);

	};
}