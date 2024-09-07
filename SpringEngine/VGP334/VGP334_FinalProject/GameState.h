#pragma once
#include <SpringEngine/Inc/SpringEngine.h>

class GameState : public SpringEngine::AppState
{
public:
	~GameState() = default;
	void Initialize()  override;
	void Terminate()  override;
	void Update(const float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	void ChangeDance();
	void ParticleEffectsAnimation();
	void Animation1();
	void Animation2();

	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::DirectionalLight mDirectionalLight;
	SpringEngine::Graphics::StandardEffect mStandardEffect;
	
	SpringEngine::Graphics::RenderObject mGround;
	
	//character 1
	SpringEngine::Graphics::ModelId mCharacterId;
	SpringEngine::Graphics::Animator mCharacterAnimator;
	SpringEngine::Graphics::RenderGroup mCharacter;

	//character 2
	SpringEngine::Graphics::ModelId mCharacterId2;
	SpringEngine::Graphics::Animator mCharacterAnimator2;
	SpringEngine::Graphics::RenderGroup mCharacter2;



	SpringEngine::Math::Vector3 moffset = SpringEngine::Math::Vector3::Zero;
	SpringEngine::Graphics::Animation mAnimation;
	SpringEngine::Graphics::Animation mAnimation2;
	float mAnimationTime = 0.0f;
	bool mDrawSkeleton = false;

	//particles
	SpringEngine::Graphics::ParticleEffect mParticleEffect;

	SpringEngine::FireworkParticleSystem mParticleSystem;
	SpringEngine::FireworkParticleSystem mParticleSystem2;
};