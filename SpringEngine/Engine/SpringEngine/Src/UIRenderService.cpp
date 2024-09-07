#include "Precompiled.h"
#include "UIRenderService.h"

#include "UIComponent.h"

using namespace SpringEngine;

void UIRenderService::Initialize()
{
}

void UIRenderService::Terminate()
{
    mUIComponents.clear();
}

void UIRenderService::Update(float deltaTime)
{
    for (UIComponent* component : mUIComponents)
    {
        component->Update(deltaTime);
    }
}

void UIRenderService::Render()
{
    for (UIComponent* component : mUIComponents)
    {
        component->Render();
    }
}

void UIRenderService::Register(UIComponent* uiComponent)
{
    auto iter = std::find(mUIComponents.begin(), mUIComponents.end(), uiComponent);
    if (iter == mUIComponents.end())
    {
        mUIComponents.push_back(uiComponent);
    }
}

void UIRenderService::Unregister(UIComponent* uiComponent)
{
    auto iter = std::find(mUIComponents.begin(), mUIComponents.end(), uiComponent);
    if (iter != mUIComponents.end())
    {
        mUIComponents.erase(iter);
    }
}