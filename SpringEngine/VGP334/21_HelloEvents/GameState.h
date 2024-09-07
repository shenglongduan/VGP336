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


	void MoveRight();
	void OnSpacePressedEvent(const SpringEngine::Event* event);

	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::DirectionalLight mDirectionalLight;
	SpringEngine::Graphics::StandardEffect mStandardEffect;

	SpringEngine::Graphics::RenderObject mball;
	SpringEngine::Graphics::RenderObject mGround;
	

	//anmation
	SpringEngine::Math::Vector3 moffset = SpringEngine::Math::Vector3::Zero;
	SpringEngine::Graphics::Animation mAnimation;
	float mAnimationTime = 0.0f;
};