#pragma once
#include "ModelManager.h"
#include "Bone.h"

namespace SpringEngine::Graphics
{
	class Animator
	{
	public:
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);
		void Stop();
		void Start();
		void ChangeSpeed(float speed);

		bool IsFinished() const;
		size_t GetAnimationCount() const;
		int GetFrameCount() const;
		Math::Matrix4 GetToParentTransform(const Bone* bone) const;

		void AddKeyFrame(int frame, std::function<void()> callback);
	private:
		ModelId mModelId = 0;
		int mClipIndex = -1;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
		bool mIsStop = false;
		float mSpeed = 1.0f;
		int mFrameCount = 0;
		std::unordered_map<int, std::function<void()>> mCallbacks;
	};
}