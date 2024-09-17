#include "Precompiled.h"
#include "Animator.h"

using namespace SpringEngine;
using namespace SpringEngine::Core;
using namespace SpringEngine::Graphics;

void Animator::Initialize(ModelId id)
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mFrameCount = (int)animClip.ticksDuration;
}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0 || mIsStop)
	{
		return;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];

	mAnimationTick += animClip.ticksPerSecond * deltaTime * mSpeed;

	if (mAnimationTick > animClip.ticksDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick > animClip.ticksDuration)
			{
				mAnimationTick -= animClip.ticksDuration;
			}
		}
		else
		{
			mAnimationTick = animClip.ticksDuration;
		}
	}
	if (mCallbacks.find((int)mAnimationTick) != mCallbacks.end()) {
		mCallbacks[(int)mAnimationTick]();
	}
}

void Animator::Stop()
{
	mIsStop = true;
}

void Animator::Start()
{
	mIsStop = false;
}

void SpringEngine::Graphics::Animator::ChangeSpeed(float speed)
{
	mSpeed = speed;
}


bool Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.ticksDuration;
}

size_t Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

int Animator::GetFrameCount() const
{
	return mFrameCount;
}

Math::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimation[bone->index].get();
	if (animation == nullptr)
	{
		return Math::Matrix4::Identity;
	}
	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();
}


void Animator::AddKeyFrame(int frame, std::function<void()> callback)
{
	mCallbacks[frame] = callback;
}