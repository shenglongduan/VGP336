#include "GameState.h"
#include <Graphics/Inc/MeshBuilder.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -4.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    mSphere = MeshBuilder::CreateSphere(120, 120, 1.0f);
    mEarth.meshBuffer.Initialize(mSphere);
    mEarth.diffuseMapId = TextureManager::Get()->LoadTexture("Moon_Diffuse.jpg");


    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mCelShadingEffect.Initialize(L"../../Assets/Shaders/CelShader.fx");
    mCelShadingEffect.SetCamera(mCamera);
    mCelShadingEffect.SetDirectionalLight(mDirectionalLight);

    mTransparencyEffect.Initialize(L"../../Assets/Shaders/Transparency.fx");
    mTransparencyEffect.SetCamera(mCamera);
    mTransparencyEffect.SetTransparencyValue(mCurrentTransparencyValue);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mCelShadingEffect.Terminate();
    mStandardEffect.Terminate();
    mTransparencyEffect.Terminate();
    mEarth.Terminate();
}

void GameState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10, Colors::Aqua);
    SimpleDraw::Render(mCamera);
    if (mUseTransparency)
    {
        mTransparencyEffect.Begin();
        mTransparencyEffect.Render(mEarth);
        mTransparencyEffect.End();
    }
    else if (mUseCelShading)
    {
        mCelShadingEffect.Begin();
        mCelShadingEffect.Render(mEarth);
        mCelShadingEffect.End();
    }
    else
    {
        mStandardEffect.Begin();
        mStandardEffect.Render(mEarth);
        mStandardEffect.End();
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Light Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f);
        ImGui::ColorEdit3("Ambient", &mDirectionalLight.ambient.x);
        ImGui::ColorEdit3("Diffuse", &mDirectionalLight.diffuse.x);
        ImGui::ColorEdit3("Specular", &mDirectionalLight.specular.x);
    }

    if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit3("Ambient##Material", &mEarth.material.ambient.x);
        ImGui::ColorEdit3("Diffuse##Material", &mEarth.material.diffuse.x);
        ImGui::ColorEdit3("Specular##Material", &mEarth.material.specular.x);
        ImGui::ColorEdit3("Emissive##Material", &mEarth.material.emissive.x);
        ImGui::DragFloat("Power##Material", &mEarth.material.power, 1.0f, 0.0f, 100.0f);
    }

    ImGui::Separator();
    ImGui::Checkbox("Use Cel Shading", &mUseCelShading);
    ImGui::SameLine();
    ImGui::Checkbox("Use Transparency Effect", &mUseTransparency);

    if (mUseCelShading)
    {
    }

    if (mUseTransparency)
    {
        ImGui::SliderFloat("Transparency Value", &mCurrentTransparencyValue, 0.0f, 1.0f);
        mTransparencyEffect.SetTransparencyValue(mCurrentTransparencyValue);
    }

    ImGui::End();
}


void GameState::UpdateCameraControl(float deltaTime)
{
    auto input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}

