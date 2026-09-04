#pragma once

#include <array>
#include <string>
#include <imgui.h>

namespace gui
{
    class cmd_panel
    {
        float _height = 200.0f;
        bool _is_mouse_hovering = false;

        std::array<char, 256> _command_buffer;
        std::string _log;

    public:
        cmd_panel();

        void process_frame();
        
        bool is_mouse_hovering() const;
    };
}
