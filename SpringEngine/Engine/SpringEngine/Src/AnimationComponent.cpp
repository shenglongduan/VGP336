#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameWorld.h"
#include "ModelComponent.h"
#include "UpdateService.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void AnimatorComponent::Initialize()
{
    ModelComponent* modelComponent = GetOwner().GetComponent<ModelComponent>();
    mAnimator.Initialize(modelComponent->GetModelId());

    UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
    updateService->Register(this);

    if (mIdle) {
        mAnimator.PlayAnimation(mStartPlayIndex, false);
        mAnimator.Stop();
    } else {
        mAnimator.PlayAnimation(mStartPlayIndex, true);
    }

}

void AnimatorComponent::Terminate()
{
    UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
    updateService->UnRegister(this);
}

void AnimatorComponent::Update(float deltaTime)
{
    mAnimator.Update(deltaTime);
}

void AnimatorComponent::DebugUI()
{
    std::string buttonTag = "PlayAnim##" + GetOwner().GetName();
    uint32_t animationCount = mAnimator.GetAnimationCount();
    for (uint32_t i = 0; i < animationCount; ++i)
    {
        std::string buttonName = buttonTag + std::to_string(i);
        if (ImGui::Button(buttonName.c_str()))
        {
            Play(i, true);
        }
    }
}

void AnimatorComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("StartPlayIndex"))
    {
        mStartPlayIndex = value["StartPlayIndex"].GetInt();
    }
    if (value.HasMember("Idle"))
    {
        mIdle = value["Idle"].GetBool();
    }
    
}

bool AnimatorComponent::Play(int index, bool looping)
{
    mAnimator.PlayAnimation(index, looping);
    return true;
}

Graphics::Animator& AnimatorComponent::GetAnimator()
{
    return mAnimator;
}

const Graphics::Animator& AnimatorComponent::GetAnimator() const
{
    return mAnimator;
}

void AnimatorComponent::AddKeyFrame(int frame, std::function<void()> callback) {
    mAnimator.AddKeyFrame(frame, callback);
}

int AnimatorComponent::GetStartPlayIndex() {
    return mStartPlayIndex;
}