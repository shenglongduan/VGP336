#include "Precompiled.h"
#include "UpdateService.h"

#include "Component.h"
using namespace SpringEngine;

void UpdateService::Update(float deltaTime)
{
	mIsUpdating = true;
	for (Component* component : mUpdateComponents)
	{
		component->Update(deltaTime);
	}

	mIsUpdating = false;
	for (Component* component : mPendingComponents)
	{
		mUpdateComponents.push_back(component);
	}
	mPendingComponents.clear();
}

void UpdateService::Register(Component* component)
{
	bool isRegistered = false;
	auto iter = std::find(mUpdateComponents.begin(), mUpdateComponents.end(), component);
	isRegistered = iter != mUpdateComponents.end();
	if (!isRegistered)
	{
		iter = std::find(mPendingComponents.begin(), mPendingComponents.end(), component);
		isRegistered = iter != mPendingComponents.end();
	}

	if (!isRegistered)
	{
		if (mIsUpdating)
		{
			mPendingComponents.push_back(component);
		}
		else
		{
			mUpdateComponents.push_back(component);
		}
	}
}

void UpdateService::UnRegister(Component* component)
{
	auto iter = std::find(mUpdateComponents.begin(), mUpdateComponents.end(), component);
	if (iter != mUpdateComponents.end())
	{
		mUpdateComponents.erase(iter);
	}
	iter = std::find(mPendingComponents.begin(), mPendingComponents.end(), component);
	if (iter != mPendingComponents.end())
	{
		mPendingComponents.erase(iter);
	}
}
