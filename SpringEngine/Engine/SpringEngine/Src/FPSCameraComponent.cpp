#include "Precompiled.h"
#include "FPSCameraComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"

#include "GameWorld.h"
#include "UpdateService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void FPSCameraComponent::Initialize()
{	 
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: game world requires and update service");
	updateService->Register(this);

	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
}	 
	 
void FPSCameraComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponent: game world requires and update service");
	updateService->UnRegister(this);

	mCameraComponent = nullptr;
}	 
	 
void FPSCameraComponent::Update(float deltaTime)
{
	Camera& camera = mCameraComponent->GetCamera();
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? mShiftSpeed : mMoveSpeed;
	const float turnSpeed = mTurnSpeed;
	if (input->IsKeyDown(KeyCode::W))
	{
		camera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		camera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		camera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void FPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}
	if (value.HasMember("TurnSpeed"))
	{
		mTurnSpeed = value["TurnSpeed"].GetFloat();
	}
	if (value.HasMember("ShiftSpeed"))
	{
		mShiftSpeed = value["ShiftSpeed"].GetFloat();
	}
}
