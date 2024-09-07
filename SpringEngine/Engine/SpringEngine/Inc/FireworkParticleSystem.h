#pragma once

#include "ParticleSystem.h"

namespace SpringEngine
{
	class Event;
	class FireworkParticle : public Particle
	{
		public:
			FireworkParticle() = default;
			~FireworkParticle() = default;
			virtual void Initialize() override;
			virtual void Terminate() override;

			virtual void Activate(const ParticleActivateData& data) override;
			virtual void Update(float deltaTime) override;

		private:
			void OnDeath();
	};

	class FireworkParticleSystem : public ParticleSystem
	{
	public:
		virtual void Terminate() override;
		virtual void SetCamera(const Graphics::Camera& camera) override;

		template<class Effect>
		void Render(Effect& effect)
		{
			if (IsActive())
			{
				ParticleInfo particleInfo;
				for (const int& index : mParticleIndexes)
				{
					FireworkParticle* particle = static_cast<FireworkParticle*>(mParticles[index].get());
					if (particle->IsActive())
					{
						particle->GetCurrentInfo(particleInfo);
						mRenderObject.transform = particle->GetTransform();
						mRenderObject.transform.scale = particleInfo.currentScale;
						effect.Render(mRenderObject, particleInfo.currentColor);
					}
				}
			}

			for (auto& e : mExplosionEffects)
			{
				e->Render(effect);
			}

		}
	protected:
		virtual void InitializeParticles(uint32_t count) override;
		void OnExplosionEvent(const Event* event);

		int mNextAvailabeEffect = 0;
		std::vector<std::unique_ptr<ParticleSystem>> mExplosionEffects;
	};
}