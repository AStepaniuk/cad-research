#pragma once

#include <imgui.h>
#include <initializer_list>

namespace gui::components
{
    enum class button_style
    {
        standard,
        warning
    };

    struct button_meta
    {
        const char* title;
        bool is_default = false; // Triggers implicitly when Enter/KeypadEnter is pressed
        button_style style = button_style::standard;
    };

    int action_strip(std::initializer_list<button_meta> buttons);
}
