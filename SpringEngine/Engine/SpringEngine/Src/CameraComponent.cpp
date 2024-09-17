#include "Precompiled.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "CameraService.h"
#include "UpdateService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void CameraComponent::Initialize()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraService != nullptr, "CameraComponent: GameWorld needs a camera service");

	cameraService->Register(this);

	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	updateService->Register(this);

	if (mIsAdhere) {
		TransformComponent* transformComponent = GetOwner().GetComponent<TransformComponent>();
		ASSERT(cameraService != nullptr, "CameraComponent: TransformComponent needs in preious with IsAdhere flag");
	}
}

void CameraComponent::Terminate()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraService != nullptr, "CameraComponent: GameWorld needs a camera service");

	cameraService->UnRegister(this);
}

void CameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		 const auto& pos = value["Position"].GetArray();
		 float x = pos[0].GetFloat();
		 float y = pos[1].GetFloat();
		 float z = pos[2].GetFloat();
		 mCamera.SetPosition({ x,y,z });

		 mRelativePosition = { x, y, z };
	}
	if (value.HasMember("LookAt"))
	{
		const auto& look = value["LookAt"].GetArray();
		float x = look[0].GetFloat();
		float y = look[1].GetFloat();
		float z = look[2].GetFloat();
		mCamera.SetLookAt({ x,y,z });
	}
	if (value.HasMember("IsAdhere"))
	{
		mIsAdhere = value["IsAdhere"].GetBool();
	}
}

void SpringEngine::CameraComponent::Update(float deltaTime)
{
	if (mIsAdhere) {
		mCamera.SetPosition(GetOwner().GetComponent<TransformComponent>()->GetPosition() + mRelativePosition);
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::ONE))
	{
		GetOwner().GetWorld().GetService<CameraService>()->SetMainCamera(0);
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::TWO))
	{
		GetOwner().GetWorld().GetService<CameraService>()->SetMainCamera(1);
	}
}

Camera& CameraComponent::GetCamera()
{
	return mCamera;
}

const Camera& CameraComponent::GetCamera() const
{
	return mCamera;
}
