#include "Precompiled.h"
#include "FireWorkParticleSystem.h"

#include "EventManager.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void FireworkParticle::Initialize()
{
	Particle::Initialize();
}

void FireworkParticle::Terminate()
{
	Particle::Terminate();
}

void FireworkParticle::Activate(const ParticleActivateData& data)
{
	Particle::Activate(data);
}

void FireworkParticle::Update(float deltaTime)
{
	if (mLifeTime > 0.0f)
	{
		Particle::Update(deltaTime);
		if (mLifeTime < 0.0f)
		{
			OnDeath();
		}
	}
}

void FireworkParticle::OnDeath()
{
	FireworkExplodeEvent feEvent;
	feEvent.position = mTransform.position;
	EventManager::Broadcast(&feEvent);

}

void FireworkParticleSystem::Terminate()
{
	ParticleSystem::Terminate();
	for (auto& ps : mExplosionEffects)
	{
		ps->Terminate();
		ps.reset();
	}

}

void FireworkParticleSystem::SetCamera(const Graphics::Camera& camera)
{
	ParticleSystem::SetCamera(camera);
}

void FireworkParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndexes.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndexes[i] = i;
		mParticles[i] = std::make_unique<FireworkParticle>();
		mParticles[i]->Initialize();
	}

	ParticleSystemInfo info;
	info.spawnDelay = 0.0f;
	info.systemLifeTime = 0.0f;
	info.minParticlePerEmit = 20;
	info.maxParticlePerEmit = 30;
	info.minSpawnAngle = -180.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 180.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minLifeTime = 0.5f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::White;
	info.maxStartColor = Colors::Yellow;
	info.minEndColor = Colors::White;
	info.maxEndColor = Colors::LightYellow;
	info.minStartScale = { 1.0f,1.0f,1.0f };
	info.maxStartScale = { 1.0f,1.0f,1.0f };
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticles = 50;
	info.particleTextureId = TextureManager::Get()->LoadTexture("sun.jpg");

	const uint32_t maxExplosionEffects = 10;
	mExplosionEffects.resize(maxExplosionEffects);
	for (uint32_t i = 0; i < maxExplosionEffects; ++i)
	{
		mExplosionEffects[i] = std::make_unique<ParticleSystem>();
		mExplosionEffects[i]->Initialize(info);
	}

	EventManager::Get()->AddListener(EventType::FireworkExplode, std::bind(&FireworkParticleSystem::OnExplosionEvent, this, std::placeholders::_1));

}

void FireworkParticleSystem::OnExplosionEvent(const Event* event)
{
	FireworkExplodeEvent* feEvent = (FireworkExplodeEvent*)event;
	auto& nextPS = mExplosionEffects[mNextAvailabeEffect];
	mNextAvailabeEffect = (mNextAvailabeEffect + 1) % mExplosionEffects.size();

	nextPS->SetPosition(feEvent->position);
	nextPS->SpawnParticles();

}