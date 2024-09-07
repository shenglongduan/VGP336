#include "Precompiled.h"
#include "TerrainEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void TerrainEffect::Initialize()
{
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Terrain.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mLightingBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightingBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TerrainEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightingBuffer.BindVS(1);
	mLightingBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void TerrainEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}

}

void TerrainEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "TerrainEffect: no camera specified");
	ASSERT(mDirectionalLight != nullptr, "TerrainEffect: no light specified");

	const Matrix4 matWorld = renderObject.transform.GetMatrix4();
	Matrix4 matView = mCamera->GetViewMatrix();
	Matrix4 matProj = mCamera->GetProjectionMatrix();

	SettingsData settingsData;
	settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
	settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
	settingsData.useBlend = mSettingsData.useBlend > 0 && renderObject.bumpMapId > 0;
	settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
	settingsData.depthBias = mSettingsData.depthBias;
	settingsData.blendHeight = mSettingsData.blendHeight;


	TransformData transformData;
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.world = Transpose(matWorld);
	transformData.viewPosition = mCamera->GetPosition();

	if (settingsData.useShadowMap > 0)
	{
		matView = mLightCamera->GetViewMatrix();
		matProj = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = Transpose(matWorld * matView * matProj);

		mShadowMap->BindPS(4);
	}

	mTransformBuffer.Update(transformData);
	mSettingsBuffer.Update(settingsData);
	mLightingBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindPS(renderObject.specMapId, 2);
	tm->BindPS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();

}

void TerrainEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("TerrainEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useNormalMap = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormal##MapTerrain", &useNormalMap))
		{
			mSettingsData.useNormalMap = useNormalMap ? 1 : 0;
		}

		bool useSpecMap = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecMap##Terrain", &useSpecMap))
		{
			mSettingsData.useSpecMap = useSpecMap ? 1 : 0;
		}
		
		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("UseShadowMap##Terrain", &useShadowMap))
		{
			mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias##Terrain", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");

		bool useBlend = mSettingsData.useBlend > 0;
		if (ImGui::Checkbox("UseBlend##Terrain", &useBlend))
		{
			mSettingsData.useBlend = useBlend ? 1 : 0;
		}

		ImGui::DragFloat("BlendHeight##Terrain", &mSettingsData.blendHeight, 0.1f, 0.0f, 20.0f);
	}

}

void TerrainEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void TerrainEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void TerrainEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
}
