#pragma once

namespace gui {
    class main_menu
    {
    public:
        void process_frame();

        bool is_mouse_hovering() const;

    private:
        bool _is_mouse_hovering = false;
    };
}