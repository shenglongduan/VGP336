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

	Mesh Ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(Ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("skysphere/sky.jpg");
	//particle
	ParticleEffectsAnimation();

	// character 1
	mCharacterId = ModelManager::Get()->LoadModel("../../Assets/Models/Finals/Samba/Samba.model");
	mCharacter = CreateRenderGroup(mCharacterId, &mCharacterAnimator);
	mCharacterAnimator.Initialize(mCharacterId);
	Animation1();


	// character 2
	mCharacterId2 = ModelManager::Get()->LoadModel("../../Assets/Models/Finals/Move_1/Move_1.model");
	ModelManager::Get()->AddAnimation(mCharacterId2, "../../Assets/Models/Jumping/Jumping.model");
	mCharacter2 = CreateRenderGroup(mCharacterId2, &mCharacterAnimator2);
	mCharacterAnimator2.Initialize(mCharacterId2);
	Animation2();

	mAnimationTime = 0.0f;
}
void GameState::Terminate()
{
	CleanupRenderGroup(mCharacter);
	mGround.Terminate();
	mStandardEffect.Terminate();
	mParticleSystem.Terminate();
	mParticleSystem2.Terminate();
	mParticleEffect.Terminate();
}
void GameState::Update(const float deltaTime)
{
	mParticleSystem.Update(deltaTime);
	mParticleSystem2.Update(deltaTime);
	UpdateCameraControl(deltaTime);
	float prevTime = mAnimationTime;
	mAnimationTime += deltaTime;
	while (mAnimationTime > mAnimation.GetDuration())
	{
		mAnimationTime -= mAnimation.GetDuration();
	}
	mAnimation.PlayEvents(prevTime, mAnimationTime);
	mAnimation2.PlayEvents(prevTime, mAnimationTime);
	mCharacterAnimator.Update(deltaTime);
	mCharacterAnimator2.Update(deltaTime);
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
	mParticleEffect.Begin();
	mParticleSystem.Render(mParticleEffect);
	mParticleSystem2.Render(mParticleEffect);
	mParticleEffect.End();
	mStandardEffect.Begin();
	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::BoneTransforms boneTransforms2;
		AnimationUtil::ComputeBoneTransforms(mCharacterId, boneTransforms, &mCharacterAnimator);
		AnimationUtil::ComputeBoneTransforms(mCharacterId2, boneTransforms2, &mCharacterAnimator2);
		AnimationUtil::DrawSkeleton(mCharacterId, boneTransforms);
		AnimationUtil::DrawSkeleton(mCharacterId2, boneTransforms2);
	}
	else
	{
		DrawRenderGroup(mStandardEffect, mCharacter);
		DrawRenderGroup(mStandardEffect, mCharacter2);
	}
	const auto& transform = mAnimation.GetTransform(mAnimationTime);
	for (auto& ro : mCharacter)
	{
		ro.transform = transform;
	}
	const auto& transform2 = mAnimation2.GetTransform(mAnimationTime);
	for (auto& ro : mCharacter2)
	{
		ro.transform = transform2;
	}
	mStandardEffect.Render(mGround);
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
	ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
	mStandardEffect.DebugUI();
	mParticleSystem.DebugUI();
	Physics::PhysicsWorld::Get()->DebugUI();
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
void GameState::ParticleEffectsAnimation()
{
	//particle
	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	ParticleSystemInfo info;
	info.spawnPosition = { -4.0f,0.0f,5.0f };
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.systemLifeTime = 99999.0f;
	info.minParticlePerEmit = 10;
	info.maxParticlePerEmit = 20;
	info.minTimeBetweenEmit = 1.0f;
	info.maxTimeBetweenEmit = 3.0f;
	info.minSpawnAngle = -10.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 10.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minLifeTime = 0.3f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::White;
	info.maxStartColor = Colors::White;
	info.minEndColor = Colors::White;
	info.maxEndColor = Colors::White;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = Math::Vector3::One;
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticles = 50;
	info.particleTextureId = TextureManager::Get()->LoadTexture("sun.jpg");

	mParticleSystem.Initialize(info);
	mParticleSystem.SetCamera(mCamera);

	ParticleSystemInfo info2;
	info2.spawnPosition = { 4.0f,0.0f,5.0f };
	info2.spawnDirection = Math::Vector3::YAxis;
	info2.spawnDelay = 0.0f;
	info2.systemLifeTime = 99999.0f;
	info2.minParticlePerEmit = 10;
	info2.maxParticlePerEmit = 20;
	info2.minTimeBetweenEmit = 1.0f;
	info2.maxTimeBetweenEmit = 3.0f;
	info2.minSpawnAngle = -10.0f * 3.141592 / 180.0f;
	info2.maxSpawnAngle = 10.0f * 3.141592 / 180.0f;
	info2.minSpeed = 5.0f;
	info2.maxSpeed = 10.0f;
	info2.minLifeTime = 0.3f;
	info2.maxLifeTime = 1.0f;
	info2.minStartColor = Colors::White;
	info2.maxStartColor = Colors::White;
	info2.minEndColor = Colors::White;
	info2.maxEndColor = Colors::White;
	info2.minStartScale = Math::Vector3::One;
	info2.maxStartScale = Math::Vector3::One;
	info2.minEndScale = Math::Vector3::Zero;
	info2.maxEndScale = Math::Vector3::Zero;
	info2.maxParticles = 50;
	info2.particleTextureId = TextureManager::Get()->LoadTexture("sun.jpg");

	mParticleSystem2.Initialize(info2);
	mParticleSystem2.SetCamera(mCamera);


}
void GameState::Animation1()
{
	AnimationCallback cb = [&]() {mCharacterAnimator.PlayAnimation(0, true); };

	mAnimation = AnimationBuilder()
		.AddEventKey(cb, 0.1f)
		.AddPositionKey({ 0.1f, 0.0f, 0.0f }, 0.1f, EaseType::Linear)
		.AddPositionKey({ 0.1f,	0.0f, 1.0f }, 1.0f, EaseType::Linear)
		.AddPositionKey({ 0.5f, 0.0f, 1.0f }, 2.0f, EaseType::Linear)
		.AddPositionKey({ 0.5f, 0.0f, 1.0f }, 4.0f)
		.AddPositionKey({ 0.5f, 0.0f, 2.0f }, 5.0f, EaseType::Linear)
		.AddPositionKey({ 1.0f, 0.0f, 2.0f }, 7.0f, EaseType::Linear)
		.AddPositionKey({ 1.0f, 0.0f, 1.0f }, 8.0f, EaseType::Linear)
		.AddPositionKey({ 1.0f, 0.0f, 2.0f }, 10.0f, EaseType::Linear)
		.AddPositionKey({ 2.0f, 0.0f, 2.0f }, 12.0f, EaseType::Linear)
		.AddPositionKey({ 2.0f, 0.0f, 0.5f }, 15.0f, EaseType::Linear)
		.AddPositionKey({ 4.0f, 0.0f, 0.5f }, 18.0f, EaseType::Linear)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 25.0f, EaseType::Linear)
		.AddEventKey([&]() {}, 45.f)
		.Build();
}
void GameState::Animation2()
{
	
	AnimationCallback cb2 = [&]() {mCharacterAnimator2.PlayAnimation(0, true); };
	AnimationCallback cb3 = [&]() {mCharacterAnimator2.PlayAnimation(1, true); };
	mAnimation2 = AnimationBuilder()
		.AddEventKey(cb3, 0.1f)
		.AddPositionKey({ 0.0f, 0.0f, -2.0f }, 0.1f, EaseType::EaseInOutQuad)
		//running left
		.AddEventKey(cb2, 1.2f)
		.AddPositionKey({ 0.0f, 0.0f, 8.0f }, 1.2f, EaseType::EaseInOutQuad)
		.AddPositionKey({ -0.5f, 0.0f, 8.0f }, 2.3f, EaseType::EaseInQuad)
		.AddPositionKey({ -0.5f, 0.0f, 8.0f }, 3.2f)
		.AddPositionKey({ -1.0f, 0.0f, 8.0f }, 6.2f, EaseType::EaseOutQuad)
		.AddPositionKey({ -2.0f, 0.0f, 8.0f }, 11.2f, EaseType::EaseOutQuad)
		.AddPositionKey({ -4.0f, 0.0f, 8.0f }, 16.2f, EaseType::EaseOutQuad)
		.AddEventKey(cb3, 16.3f)
		.AddPositionKey({ -4.0f, 0.0f, 0.0f }, 17.5f, EaseType::EaseOutQuad)
		.AddEventKey(cb2, 17.6f)
		.AddEventKey([&]() {}, 45.f)
		//turning
		.AddRotationKey(Math::Quaternion::Normalize({ 0.0f,10.0f,0.0f,1.0f }), 0.1f)
		.AddRotationKey(Math::Quaternion::Normalize({ 0.0f,5.0f,0.0f,1.0f }), 5.0f)
		.AddRotationKey(Math::Quaternion::Normalize({ 0.0f,2.0f,0.0f,1.0f }), 10.0f)
		.AddRotationKey(Math::Quaternion::Normalize({ 0.0f,0.0f,0.0f,1.0f }), 15.0f)
		.Build();
}