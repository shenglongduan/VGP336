#include "Precompiled.h"
#include "ThrowComponent.h"

#include "GameWorld.h"
#include "ModelComponent.h"
#include "UpdateService.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"


using namespace SpringEngine;
using namespace SpringEngine::Graphics;
static float getRandomNumber() {
    std::vector<float> numbers = { 0.5f, 1.0f, 1.5f };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numbers.size() - 1);
    return numbers[dis(gen)];
}
void ThrowComponent::Initialize()
{

    UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
    updateService->Register(this);

    AnimatorComponent* animatorComponent = GetOwner().GetComponent<AnimatorComponent>();
    mStartPlayIndex = animatorComponent->GetStartPlayIndex();
    mAnimator = &animatorComponent->GetAnimator();
    mAnimator->AddKeyFrame(mAnimator->GetFrameCount() - 1, [&]() {mLock = false;});
    if (mThrowObject) {
        mAnimator->AddKeyFrame(mThrowFrame, [&](){
            RigidBodyComponent* rigidBody = mThrowObject->GetComponent<RigidBodyComponent>();
            rigidBody->SetVelocity(mThrowVelocity * getRandomNumber());
            rigidBody->SetPosition(mThrowPosition);
        });
    }
}

void ThrowComponent::Terminate()
{
    UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
    updateService->UnRegister(this);
}

void ThrowComponent::Update(float deltaTime)
{
    if (!mLock && Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
	{
        mLock = true;
        mAnimator->Start();
        mAnimator->PlayAnimation(mStartPlayIndex, false);
	}

    // if active
    // camera component
    // cameracomponent->syncposition()
}

void ThrowComponent::DebugUI()
{
    std::string buttonTag = "PlayThrowAnim##" + GetOwner().GetName();
}

void ThrowComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("ThrowFrame"))
    {
        mThrowFrame = value["ThrowFrame"].GetInt();
    }
    if (value.HasMember("ThrowVelocity"))
    {
        const auto& v = value["ThrowVelocity"].GetArray();
        mThrowVelocity.x = v[0].GetFloat();
        mThrowVelocity.y = v[1].GetFloat();
        mThrowVelocity.z = v[2].GetFloat();
    }
    if (value.HasMember("ThrowPosition"))
    {
        const auto& p = value["ThrowPosition"].GetArray();
        mThrowPosition.x = p[0].GetFloat();
        mThrowPosition.y = p[1].GetFloat();
        mThrowPosition.z = p[2].GetFloat();
    }
    if (value.HasMember("ThrowObject"))
    {
        mThrowObject = GetOwner().GetWorld().GetGameObject(value["ThrowObject"].GetString());
    }
    
}