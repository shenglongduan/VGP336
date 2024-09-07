#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	// create shapes in NDC normalized device coordinate
	// -1 to 1 (x, y, z)
	MeshPX mesh = MeshBuilder::CreateSkySpherePX(60, 60, 10.0f);

	auto device = GraphicsSystem::Get()->GetDevice();
	// Create a vertex buffer
	mMeshBuffer.Initialize(mesh);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";

	// Create a vertex shader
	mVertexShader.Initialize<VertexPX>(shaderFile);

	// Create a pixel shader
	mPixelShader.Initialize(shaderFile);

	mCamera.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mConstantBuffer.Initialize(sizeof(Matrix4));

	mDiffuseTexture.Initialize(L"../../Assets/Textures/skysphere/sky.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	mDiffuseTexture.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

float totalTime = 0.0f;
float yRot = 0.0f;
float xRot = 0.0f;
Vector3 position(0.0f);
void GameState::Update(float deltaTime)
{
	totalTime += deltaTime;
	yRot += 0.1f * deltaTime;
	//xRot += 0.1f * deltaTime;
	//position.y = std::sin(totalTime);
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mDiffuseTexture.BindPS(0);
	mSampler.BindPS(0);

	Matrix4 matWorld = Matrix4::RotationX(xRot) * Matrix4::RotationY(yRot) * Matrix4::Translation(position);
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();
	Matrix4 matFinal = matWorld * matView * matProj;
	Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}
