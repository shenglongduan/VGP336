#include "Precompiled.h"
#include "ParticleSyetemEffect.h"

#include "RenderObject.h"
#include "Camera.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void ParticleEffect::Initialize()
{
	std::filesystem::path filePath = L"../../Assets/Shaders/Particle.fx";
	mVertexShader.Initialize<Vertex>(filePath);
	mPixelShader.Initialize(filePath);
	mParticleBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void ParticleEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();
	mParticleBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ParticleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mParticleBuffer.BindVS(0);
	mParticleBuffer.BindPS(0);
	mSampler.BindPS(0);
	mBlendState.Set();

}

void ParticleEffect::End()
{
	mBlendState.ClearState();
}

void ParticleEffect::Render(const RenderObject& renderObject)
{
	Render(renderObject, Colors::White);
}

void ParticleEffect::Render(const RenderObject& renderObject, const Color& color)
{
	Matrix4 matView = mCamera->GetViewMatrix();
	Matrix4 matScale = Matrix4::Scaling(renderObject.transform.scale);
	Matrix4 matTransform = Matrix4::Translation(TransformCoord(renderObject.transform.position, matView));
	Matrix4 matCamLocal = matScale * matTransform;
	Matrix4 matProj = mCamera->GetProjectionMatrix();
	Matrix4 matFinal = matCamLocal * matProj;

	ParticleData data;
	data.wvp = Transpose(matFinal);
	data.color = color;
	mParticleBuffer.Update(data);

	TextureManager::Get()->BindPS(renderObject.diffuseMapId, 0);

	renderObject.meshBuffer.Render();
}

void ParticleEffect::DebugUI()
{
	// not atm
}

void ParticleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
