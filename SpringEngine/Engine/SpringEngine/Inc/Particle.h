#pragma once

namespace SpringEngine
{
	struct ParticleActivateData
	{
		float lifeTime = 0.0f;
		Color startColor = Colors::White;
		Color endColor = Colors::White;
		Math::Vector3 startScale = Math::Vector3::One;
		Math::Vector3 endScale = Math::Vector3::One;
		Math::Vector3 position = Math::Vector3::Zero;
		Math::Vector3 velocity = Math::Vector3::Zero;
	};

	struct ParticleInfo
	{
		Color currentColor = Colors::White;
		Math::Vector3 currentScale = Math::Vector3::One;
	};

	class Particle
	{
	public:
		Particle() = default;
		virtual ~Particle() = default;

		virtual void Initialize();
		virtual void Terminate();

		virtual void Activate(const ParticleActivateData& date);
		virtual void Update(float deltaTime);

		bool IsActive() const;
		void GetCurrentInfo(ParticleInfo& info) const;
		const Graphics::Transform& GetTransform() const;

	protected:
		Graphics::Transform mTransform;
		Physics::RigidBody mRigidBody;
		Physics::CollisionShape mCollisionShape;

		ParticleActivateData mDate;
		float mLifeTime = 0.0f;

	};

}