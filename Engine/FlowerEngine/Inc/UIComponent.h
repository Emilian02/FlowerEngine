#pragma once


#include "Component.h"

namespace FlowerEngine
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}