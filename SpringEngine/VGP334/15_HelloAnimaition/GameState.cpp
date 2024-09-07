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


	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);


	Mesh ball = MeshBuilder::CreateSphere(60, 60, 1.0f);
	mball.meshBuffer.Initialize(ball);
	mball.diffuseMapId = TextureManager::Get()->LoadTexture("misc/basketball.jpg");

	Mesh Ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(Ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");


	mAnimation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 5.0f, 0.0f }, 0.0f, EaseType::EaseInOutQuad)
		.AddPositionKey({ 0.0f, 0.5f, 0.0f }, 1.0f, EaseType::EaseInQuad)
		.AddPositionKey({ 0.0f, 0.5f, 0.0f }, 1.1f)
		.AddPositionKey({ 0.0f, 5.0f, 0.0f }, 2.0f, EaseType::EaseOutQuad)
		.AddRotationKey({ 0.0f,0.0f,0.0f,1.0f }, 0.0f)
		.AddRotationKey(Math::Quaternion::Normalize({ 6.28f,0.0f,0.0f,1.0f }), 2.0f)
		.AddScaleKey({ 1.0f, 1.0f, 1.0f},0.0f)
		.AddScaleKey({ 1.0f, 1.0f, 1.0f},0.8f)
		.AddScaleKey({ 1.0f, 0.25f, 1.0f},1.0f)
		.AddScaleKey({ 1.0f, 0.25f, 1.0f},1.1f)
		.AddScaleKey({ 1.0f, 1.5f, 1.0f},1.25f)
		.AddScaleKey({ 1.0f, 1.0f, 1.0f},2.0f)
		.Build();


	mAnimationTime = 0.0f;
}
void GameState::Terminate()
{
	mGround.Terminate();
	mball.Terminate();
	mStandardEffect.Terminate();
}
void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);

	mAnimationTime += deltaTime;
	while (mAnimationTime > mAnimation.GetDuration())
	{
		mAnimationTime -= mAnimation.GetDuration();
	}
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mball.transform = mAnimation.GetTransform(mAnimationTime);
	mStandardEffect.Begin();
		mStandardEffect.Render(mGround);
		mStandardEffect.Render(mball);
	mStandardEffect.End();

}
void GameState::DebugUI()
{
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
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