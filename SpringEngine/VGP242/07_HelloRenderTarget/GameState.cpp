#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mRenderObject.meshBuffer.Initialize(sphere);
	mRenderObject.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/misc/basketball.jpg");
	MeshPX sky = MeshBuilder::CreateSkySpherePX(60, 60, 10.0f);
	mRenderObject2.meshBuffer.Initialize(sky);
	mRenderObject2.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/misc/concrete.jpg");

	mSimpleEffect.Initialize();
	mSimpleEffect.SetCamera(mCamera);

	mRenderTargetSimpleEffect.Initialize();
	mRenderTargetSimpleEffect.SetCamera(mRenderTargetCamera);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
	mRenderTargetSimpleEffect.Terminate();
	mRenderTarget.Terminate();
	mSimpleEffect.Terminate();
	mRenderObject2.Terminate();
	mRenderObject.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
		mRenderTargetSimpleEffect.Begin();
			mRenderTargetSimpleEffect.Render(mRenderObject);
			mRenderTargetSimpleEffect.Render(mRenderObject2);
		mRenderTargetSimpleEffect.End();
	mRenderTarget.EndRender();

	mSimpleEffect.Begin();
		mSimpleEffect.Render(mRenderObject);
		mSimpleEffect.Render(mRenderObject2);
	mSimpleEffect.End();
}

Math::Vector3 rotation = Math::Vector3::Zero;
void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
	Math::Vector3 pos = mRenderObject.transform.position;
	if (ImGui::DragFloat3("Position##", &pos.x, 0.1f))
	{
		mRenderObject.transform.position = pos;
	}
	if (ImGui::DragFloat3("Rotation##", &rotation.x, 0.01f))
	{
		mRenderObject.transform.rotation = Math::RotationEuler(rotation);
	}
	Math::Vector3 scale = mRenderObject.transform.scale;
	if (ImGui::DragFloat3("Scale##", &scale.x, 0.1f, 0.1f, 10.0f))
	{
		mRenderObject.transform.scale = scale;
	}

	ImGui::Separator();
	ImGui::Text("RenderTarget");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	);

	ImGui::End();
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
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
