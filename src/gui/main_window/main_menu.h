#pragma once

namespace gui {
    class main_menu
    {
    public:
        enum class item { none, add_wall };

        void process_frame();

        bool is_mouse_hovering() const;

        item choosen_item() const;

    private:
        bool _is_mouse_hovering = false;
        item _choosen_item = item::none;
    };
}