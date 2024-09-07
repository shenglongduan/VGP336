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

	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::DirectionalLight mDirectionalLight;
	SpringEngine::Graphics::StandardEffect mStandardEffect;

	SpringEngine::Graphics::RenderGroup mCuteman;
	SpringEngine::Graphics::RenderGroup mShortHair;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;
};