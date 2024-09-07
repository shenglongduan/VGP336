#include "Precompiled.h"
#include "Particle.h"

using namespace SpringEngine;
using namespace SpringEngine::Math;
using namespace SpringEngine::Graphics;


void Particle::Initialize()
{
	mLifeTime = 0.0f;
	mCollisionShape.InitializeEmpty();
	mTransform.position = { static_cast<float>(std::rand()),static_cast<float>(std::rand()) ,static_cast<float>(std::rand()) };
	mRigidBody.Initialize(mTransform, mCollisionShape, 0.1f);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mCollisionShape.Terminate();
}

void Particle::Activate(const ParticleActivateData& date)
{
	mDate = date;
	mLifeTime = date.lifeTime;
	mRigidBody.SetPosition(date.position);
	mRigidBody.SetVelocity(date.velocity);
}

void Particle::Update(float deltaTime)
{
	mLifeTime -= deltaTime;
}

bool Particle::IsActive() const
{
	return mLifeTime > 0.0f;
}

void Particle::GetCurrentInfo(ParticleInfo& info) const
{
	if (mDate.lifeTime > 0.0f)
	{
		float t = (1.0f - Clamp(mLifeTime / mDate.lifeTime, 0.0f, 1.0f));
		info.currentColor = Lerp(mDate.startColor, mDate.endColor, t);
		info.currentScale = Lerp(mDate.startScale, mDate.endScale, t);
	}

}

const Transform& Particle::GetTransform() const
{
	return mTransform;
}
