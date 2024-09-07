#include "Precompiled.h"
#include "AudioSystem.h"

using namespace DirectX;
using namespace SpringEngine;
using namespace SpringEngine::Audio;

namespace
{
	std::unique_ptr<AudioSystem> sAudioSystem;
}

void AudioSystem::StaticInitialize()
{
	ASSERT(sAudioSystem == nullptr, "AudioSystem: is already initialized");
	sAudioSystem = std::make_unique<AudioSystem>();
	sAudioSystem->Initialize();
}
void AudioSystem::AudioSystem::StaticTerminate()
{
	if (sAudioSystem != nullptr)
	{
		sAudioSystem->Terminate();
		sAudioSystem.reset();
	}
}
AudioSystem* AudioSystem::Get()
{
	ASSERT(sAudioSystem != nullptr, "AudioSystem: is not initialized");
	return sAudioSystem.get();
}

AudioSystem::AudioSystem()
{
	
}
AudioSystem::~AudioSystem()
{
	ASSERT(mAudioEngine == nullptr, "AudioSystem: terminate must be called");
}


void AudioSystem::Initialize()
{
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if defined(_DEBUG)
	flags |= AudioEngine_Debug;
#endif
	mAudioEngine = new AudioEngine(flags);
}
void AudioSystem::Terminate()
{
	if (mAudioEngine != nullptr)
	{
		delete mAudioEngine;
		mAudioEngine = nullptr;
	}
}

void AudioSystem::Update()
{
	if (mAudioEngine != nullptr && mAudioEngine->Update())
	{
		//if not audio device active
		if (mAudioEngine->IsCriticalError())
		{
			LOG("AudioSystem: critical error, shutting down");
			Terminate();
		}
	}
}

void AudioSystem::Suspend()
{
	ASSERT(mAudioEngine != nullptr, "AudioSystem: audio engine is not avaiable");
	mAudioEngine->Suspend();
}