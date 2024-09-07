#pragma once

#include "UIComponent.h"

namespace SpringEngine
{
    class UISpriteComponent : public UIComponent
    {
    public:
        SET_TYPE_ID(ComponentId::UISprite);

        void Initialize() override;
        void Terminate() override;
        void Render() override;
        void Deserialize(const rapidjson::Value& value);

    private:
        std::filesystem::path mTexturePath;
        Graphics::UISprite mUISprite;
    };
}