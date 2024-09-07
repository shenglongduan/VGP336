#pragma once

namespace SpringEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeEmpty();
		void InitializeBox(const SpringEngine::Math::Vector3& halfExtents);
		void InitializeSphere(float radius);
		void InitializeHull(const SpringEngine::Math::Vector3& halfExtents, const SpringEngine::Math::Vector3& origin);
		void Terminate();

	private:
		btCollisionShape* mCollisionShape = nullptr;

		friend class RigidBody;
		btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
	};
}