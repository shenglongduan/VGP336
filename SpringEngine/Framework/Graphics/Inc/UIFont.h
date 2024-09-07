#pragma once

#include "Colors.h"


namespace SpringEngine::Graphics
{
    class UIFont final
    {
    public:
        enum class FontType
        {
            Arial,
            CourierNew,
            Consolas
        };

        static void StaticInitialize(FontType font);
        static void StaticTerminate();
        static UIFont* Get();

        UIFont() = default;
        ~UIFont();

        void Initialize(FontType font);
        void Terminate();

        void DrawString(const wchar_t* str, const Math::Vector2& position, float size, const Color& color);
        float GetStringWidth(const wchar_t* str, float size) const;

    private:
        FontType mFontType = FontType::Arial;
        IFW1Factory* mFontFactory = nullptr;
        IFW1FontWrapper* mFontWrapper = nullptr;
    };
}