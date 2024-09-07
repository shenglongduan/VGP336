#include"GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mSphere = MeshBuilder::CreateSphere(120, 120, 1.0f);

	mEarth.meshBuffer.Initialize(mSphere);
	mEarth.diffuseMapId = TextureManager::Get()->LoadTexture("Moon_Diffuse.jpg");
	mEarth.normalMapId = TextureManager::Get()->LoadTexture("Moon's normal.jpg");
	mEarth.specMapId = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mEarth.bumpMapId = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mEarth.material.emissive = { 0.2f,0.2f,0.2f,1.0f };
	mEarth.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mEarth.material.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mEarth.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.power = 10.0f;

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);	

	mCelShadingEffect.Initialize(L"../../Assets/Shaders/CelShader.fx");
	mCelShadingEffect.SetCamera(mCamera);
	mCelShadingEffect.SetDirectionalLight(mDirectionalLight);

	mRenderTargetCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f,0.0f,0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U8);
}
void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mStandardEffect.Terminate();
	mCelShadingEffect.Terminate();
	mEarth.Terminate();
}
void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}
void GameState::Render()
{
	mStandardEffect.SetCamera(mRenderTargetCamera);
	mRenderTarget.BeginRender();
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.End();
	mRenderTarget.EndRender();

	if (mUseCelShading)
	{
		mCelShadingEffect.Begin();
		mCelShadingEffect.Render(mEarth);
		mCelShadingEffect.End();
	}
	else 
	{
		mStandardEffect.SetCamera(mCamera);
		mStandardEffect.Begin();
		mStandardEffect.Render(mEarth);
		mStandardEffect.End();
	}

	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.End();
}
void GameState::DebugUI()
{
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction",&mDirectionalLight.direction.x,0.1f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mEarth.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mEarth.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mEarth.material.specular.r);
		ImGui::ColorEdit4("Emissive##Material", &mEarth.material.emissive.r);
		ImGui::DragFloat("Power##Material", &mEarth.material.power, 1.0f, 1.0f, 100.0f);
	}
	ImGui::Separator();
	ImGui::Text("RenderTarget");
	ImGui::Image(mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0,0 },
		{ 1,1 },
		{ 1,1,1,1 },
		{ 1,1,1,1 });
	ImGui::Separator();
	ImGui::Checkbox("UseCelShader", &mUseCelShading);
	if (mUseCelShading)
	{
		mCelShadingEffect.DebugUI();
	}
	else
	{
		mStandardEffect.DebugUI();
	}
	mStandardEffect.DebugUI();
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