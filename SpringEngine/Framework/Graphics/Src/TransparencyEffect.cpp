#include "Precompiled.h"
#include "TransparencyEffect.h"
#include "GraphicsSystem.h"
#include "TextureManager.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void TransparencyEffect::Initialize(const std::filesystem::path& filePath)
{
    mTransformBuffer.Initialize();
    mVertexShader.Initialize<Vertex>(filePath);
    mPixelShader.Initialize(filePath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mSettingsBuffer.Initialize();
    mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void TransparencyEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mTransformBuffer.Terminate();
    mSettingsBuffer.Terminate();
    mBlendState.Terminate();
}

void TransparencyEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);
    mBlendState.Set();
}

void TransparencyEffect::End()
{
    BlendState::ClearState();
}

void TransparencyEffect::Render(const RenderObject& renderObject)
{
    const auto& matWorld = renderObject.transform.GetMatrix4();
    const auto& matView = mCamera->GetViewMatrix();
    const auto& matProj = mCamera->GetProjectionMatrix();

    TransformData data;
    data.wvp = Transpose(matWorld * matView * matProj);
    data.world = Transpose(matWorld);
    data.ViewPosition = mCamera->GetPosition();

    mTransformBuffer.Update(data);

    mSettingsBuffer.BindPS(0);
    mTransformBuffer.BindVS(1);

    auto tm = TextureManager::Get();
    if (renderObject.diffuseMapId > 0) tm->BindPS(renderObject.diffuseMapId, 0);

    renderObject.meshBuffer.Render();
}

void TransparencyEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void TransparencyEffect::SetTransparencyValue(float value)
{
   
    SettingsData settingsData;
    settingsData.transparency = value;
    mSettingsBuffer.Update(settingsData);
}

void TransparencyEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("TransparencyEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static float transparency = 1.0f;
        if (ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f))
        {
            SetTransparencyValue(transparency);
        }
    }
}
