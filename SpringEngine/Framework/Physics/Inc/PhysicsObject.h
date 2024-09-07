#pragma once

class btRigidBody;
class btSoftBody;

namespace SpringEngine::Physics
{
	class PhysicsObject
	{
	public:
			PhysicsObject() = default;
			virtual ~PhysicsObject() = default;
	protected:
		friend class PhysicsWorld;

		virtual void Update() = 0;
		virtual btRigidBody* GetRigidBody() { return nullptr; }
		virtual btSoftBody* GetSoftBody() { return nullptr; }
	};
}