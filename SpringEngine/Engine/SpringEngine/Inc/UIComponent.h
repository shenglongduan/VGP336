#pragma once

#include "Component.h"

namespace SpringEngine
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}