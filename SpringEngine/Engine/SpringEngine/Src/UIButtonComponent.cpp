#include "Precompiled.h"
#include "UIButtonComponent.h"
#include "UIRenderService.h"
#include "GameWorld.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Input;

void UIButtonComponent::Initialize()
{
    for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
    {
        if (!mButtonStateTextures[i].empty())
        {
            mButtonStates[i].Initialize(mButtonStateTextures[i]);
        }
    }
    mPosition = mButtonStates[0].GetPosition();

    auto uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Register(this);
}

void UIButtonComponent::Terminate()
{
    auto uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Unregister(this);
}

void UIButtonComponent::Update(float deltaTime)
{
    mCurrentState = ButtonState::Default;

    auto input = InputSystem::Get();
    const int mouseX = input->GetMouseScreenX();
    const int mouseY = input->GetMouseScreenY();
    if (mButtonStates[static_cast<uint32_t>(mCurrentState)].IsInSprite(mouseX, mouseY))
    {
        mCurrentState = ButtonState::Hover;
        if (input->IsMouseDown(MouseButton::LBUTTON))
        {
            mCurrentState = ButtonState::Click;
        }
        if (input->IsMousePressed(MouseButton::LBUTTON))
        {
            OnClick();
        }
    }
}

void UIButtonComponent::Render()
{
    uint32_t buttonState = static_cast<uint32_t>(mCurrentState);
    if (!mButtonStateTextures[buttonState].empty())
    {
        UISpriteRenderer::Get()->Render(&mButtonStates[buttonState]);
    }
}

void UIButtonComponent::Deserialize(const rapidjson::Value& value)
{
    for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
    {
        ButtonState state = (ButtonState)i;
        std::string buttonStateString = "";
        switch (state)
        {
        case ButtonState::Default: buttonStateString = "Default"; break;
        case ButtonState::Hover: buttonStateString = "Hover"; break;
        case ButtonState::Click: buttonStateString = "Click"; break;
        case ButtonState::Disabled: buttonStateString = "Disabled"; break;
        default:
            break;
        }

        if (!value.HasMember(buttonStateString.c_str()))
        {
            continue;
        }

        auto buttonStateObj = value[buttonStateString.c_str()].GetObj();
        if (buttonStateObj.HasMember("Texture"))
        {
            mButtonStateTextures[i] = buttonStateObj["Texture"].GetString();
        }

        if (buttonStateObj.HasMember("Scale"))
        {
            auto scale = buttonStateObj["Scale"].GetArray();
            const float x = scale[0].GetFloat();
            const float y = scale[1].GetFloat();
            mButtonStates[i].SetScale({ x, y });
        }
        if (buttonStateObj.HasMember("Color"))
        {
            auto color = buttonStateObj["Color"].GetArray();
            const float r = color[0].GetFloat();
            const float g = color[1].GetFloat();
            const float b = color[2].GetFloat();
            const float a = color[3].GetFloat();
            mButtonStates[i].SetColor({ r, g, b, a });
        }
        if (buttonStateObj.HasMember("Flip"))
        {
            std::string flip = buttonStateObj["Flip"].GetString();
            if (flip == "None")
            {
                mButtonStates[i].SetFlip(Flip::None);
            }
            else if (flip == "Horizontal")
            {
                mButtonStates[i].SetFlip(Flip::Horizontal);
            }
            else if (flip == "Vertical")
            {
                mButtonStates[i].SetFlip(Flip::Vertical);
            }
            else if (flip == "Both")
            {
                mButtonStates[i].SetFlip(Flip::Both);
            }
            else
            {
                ASSERT(false, "UISpriteComponent: invalid flip %s", flip.c_str());
            }
        }
    }
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        const float x = pos[0].GetFloat();
        const float y = pos[1].GetFloat();
        for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
        {
            mButtonStates[i].SetPosition({ x,y });
        }
    }
    if (value.HasMember("Rotation"))
    {
        const float rotation = value["Rotation"].GetFloat();
        for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
        {
            mButtonStates[i].SetRotation(rotation);
        }
    }
    if (value.HasMember("Pivot"))
    {
        std::string pivot = value["Pivot"].GetString();
        Pivot buttonPivot = Pivot::TopLeft;
        if (pivot == "TopLeft")
        {
            buttonPivot = Pivot::TopLeft;
        }
        else if (pivot == "Top")
        {
            buttonPivot = Pivot::Top;
        }
        else if (pivot == "TopRight")
        {
            buttonPivot = Pivot::TopRight;
        }
        else if (pivot == "Left")
        {
            buttonPivot = Pivot::Left;
        }
        else if (pivot == "Center")
        {
            buttonPivot = Pivot::Center;
        }
        else if (pivot == "Right")
        {
            buttonPivot = Pivot::Right;
        }
        else if (pivot == "BottomLeft")
        {
            buttonPivot = Pivot::BottomLeft;
        }
        else if (pivot == "Bottom")
        {
            buttonPivot = Pivot::Bottom;
        }
        else if (pivot == "BottomRight")
        {
            buttonPivot = Pivot::BottomRight;
        }
        else
        {
            ASSERT(false, "UISpriteComponent: invalid pivot %s", pivot.c_str());
        }
        for (uint32_t i = 0; i < static_cast<uint32_t>(ButtonState::Count); ++i)
        {
            mButtonStates[i].SetPivot(buttonPivot);
        }
    }
}

void UIButtonComponent::SetCallback(ButtonCallback cb)
{
    mCallback = cb;
}

void UIButtonComponent::OnClick()
{
    LOG("BUTTON WAS CLICKED!!!!");
    if (mCallback != nullptr)
    {
        mCallback();
    }
}