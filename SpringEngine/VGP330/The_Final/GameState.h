#pragma once

#include <SpringEngine/Inc/SpringEngine.h>

class GameState : public SpringEngine::AppState
{
public:
    ~GameState() override = default;

    void Initialize() override;
    void Terminate() override;
    void Update(const float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    void UpdateCameraControl(float deltaTime);

    SpringEngine::Graphics::Camera mCamera;
    SpringEngine::Graphics::DirectionalLight mDirectionalLight;
    SpringEngine::Graphics::Mesh mSphere;
    SpringEngine::Graphics::RenderObject mEarth;
    SpringEngine::Graphics::StandardEffect mStandardEffect;

    SpringEngine::Graphics::Camera mRenderTargetCamera;
    SpringEngine::Graphics::RenderTarget mRenderTarget;

    bool mUseCelShading = false;
    SpringEngine::Graphics::StandardEffect mCelShadingEffect;

    SpringEngine::Graphics::TransparencyEffect mTransparencyEffect; 
    float mCurrentTransparencyValue = 1.0f; 
    bool mUseTransparency = false;
};
