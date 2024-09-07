#include "CustomDebugDrawComponent.h"

#include "CustomDebugDrawDisplayService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
void CustomDebugDrawComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	CustomDebugDrawDisplayService* displaySevice = GetOwner().GetWorld().GetService<CustomDebugDrawDisplayService>();
	displaySevice->Register(this);

	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	updateService->Register(this);
}

void CustomDebugDrawComponent::Terminate()
{
	CustomDebugDrawDisplayService* displaySevice = GetOwner().GetWorld().GetService<CustomDebugDrawDisplayService>();
	displaySevice->Unregister(this);
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	updateService->Register(this);
}

void CustomDebugDrawComponent::Update(float deltaTime)
{
	RigidBodyComponent* rigidBody = GetOwner().GetComponent<RigidBodyComponent>();
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
	{
		SoundBankComPent* soundbank = GetOwner().GetComponent<SoundBankComPent>();
		if (soundbank != nullptr)
		{
			soundbank->Play("Gun");
		}
		if (rigidBody != nullptr)
		{
			rigidBody->SetVelocity({ 0.0f, 10.0f, 0.0f });
		}
	}

	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::UP))
	{
		rigidBody->SetVelocity({ 1.0f, 0.0f, 0.0f });
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::DOWN))
	{
		rigidBody->SetVelocity({ -1.0f, 0.0f, 0.0f });
	}

}

void CustomDebugDrawComponent::AddDebugDraw()
{
	Vector3 worldSpace = mPosition;
	if (mTransformComponent != nullptr)
	{
		Matrix4 matWolrd = mTransformComponent->GetMatrix4();
		worldSpace = TransformCoord(mPosition, matWolrd);

	}
	SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}

void CustomDebugDrawComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Slices"))
	{
		mSlices = static_cast<uint32_t>(value["Slices"].GetInt());
	}
	if (value.HasMember("Rings"))
	{
		mRings = static_cast<uint32_t>(value["Rings"].GetInt());
	}
	if (value.HasMember("Radius"))
	{
		mRadius = value["Radius"].GetFloat();
	}
	if (value.HasMember("Position"))
	{
		auto pos = value["Position"].GetArray();
		mPosition.x = pos[0].GetFloat();
		mPosition.y = pos[1].GetFloat();
		mPosition.z = pos[2].GetFloat();
	}
	if (value.HasMember("Color"))
	{
		auto color = value["Color"].GetArray();
		mColor.r = color[0].GetFloat();
		mColor.g = color[1].GetFloat();
		mColor.b = color[2].GetFloat();
		mColor.a = color[3].GetFloat();
	}
}
