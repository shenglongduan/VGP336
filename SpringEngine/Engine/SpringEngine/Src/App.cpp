#include "Precompiled.h"
#include "App.h"
#include "AppState.h"
#include "EventManager.h"

using namespace SpringEngine;
using namespace SpringEngine::Core;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Input;
using namespace SpringEngine::Physics;
using namespace SpringEngine::Audio;

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

void App::Run(const AppConfig& config)
{
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);

	PhysicsWorld::Settings physicsSettings;

	auto handle = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handle, false);
	InputSystem::StaticInitialize(handle);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize(config.debugDrawLimit);
	TextureManager::StaticInitialize("../../Assets/Textures/");
	ModelManager::StaticInitialize();
	PhysicsWorld::StaticInitialize(physicsSettings);
	EventManager::StaticInitialize();
	AudioSystem::StaticInitialize();
	SoundEffectManager::StaticInitialize("../../Assets/Sounds");
	UISpriteRenderer::StaticInitialize();
	UIFont::StaticInitialize(UIFont::FontType::Arial);

	ASSERT(mCurrentState, "App -- no app state found");
	mCurrentState->Initialize();
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();
		
		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (!myWindow.IsActive() || inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}
		
		auto deltaTime = TimeUtil::GetDeltaTime();
		if (deltaTime < 0.5f)
		{
			//physics now is a service
			//PhysicsWorld::Get()->Update(deltaTime);

			mCurrentState->Update(deltaTime);
		}

		auto gs = GraphicsSystem::Get();
		auto sr = UISpriteRenderer::Get();
		gs->BeginRender();
			sr->BeginRender();
				mCurrentState->Render();
			sr->EndRender();
			DebugUI::BeginRender();
				mCurrentState->DebugUI();
			DebugUI::EndRender();
		gs->EndRender();
	}

	mCurrentState->Terminate();

	UIFont::StaticTerminate();
	UISpriteRenderer::StaticTerminate();
	EventManager::StaticTerminate();
	PhysicsWorld::StaticTerminate();
	ModelManager::StaticTerminate();
	TextureManager::StaticTerminate();
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
