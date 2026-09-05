#pragma once

#include <array>
#include <string>
#include <optional>

#include <imgui.h>

namespace gui
{
    class cmd_panel
    {
        float _height = 200.0f;
        bool _is_mouse_hovering = false;

        std::array<char, 256> _command_buffer;
        std::string _log;

        std::optional<std::string> _entered_command;

    public:
        cmd_panel();

        void process_frame();
        
        bool is_mouse_hovering() const;

        std::optional<std::string> take_entered_command();
        void info(std::string_view data);
        void warning(std::string_view data);
        void error(std::string_view data);
    };
}
