#pragma once
#include <SpringEngine/Inc/SpringEngine.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

class GameState : public SpringEngine::AppState
{
public:
	virtual ~GameState() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	Camera mCamera;
	RenderObject mRenderObject;
	RenderObject mRenderObject2;
	SimpleEffect mSimpleEffect;

	Camera mRenderTargetCamera;
	SimpleEffect mRenderTargetSimpleEffect;
	RenderTarget mRenderTarget;
};