#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "BlendState.h"

namespace SpringEngine::Graphics
{
    struct TransformData
    {
        Math::Matrix4 wvp;
        Math::Matrix4 world;
        Math::Vector3 ViewPosition;
        float padding;
    };

    struct SettingsData
    {
        float transparency;
        float padding[3]; 
    };

    class Camera;
    class RenderObject;
    class Texture;

    class TransparencyEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);
        void SetTransparencyValue(float value);

        void DebugUI();

    private:
        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;
        Sampler mSampler;
        BlendState mBlendState;

        const Camera* mCamera = nullptr;
    };
}
