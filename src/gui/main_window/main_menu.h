#pragma once

#include <imgui.h>

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace gui {
    class main_menu
    {
    public:
        enum class item { none, add_wall };

        main_menu();

        void process_frame();

        bool is_mouse_hovering() const;

        item choosen_item() const;

    private:
        bool _is_mouse_hovering = false;
        item _choosen_item = item::none;


        struct menu_item_meta
        {
            std::string caption;
            std::optional<ImGuiKey> hotkey;
            std::string hotkey_text;

            item item_choice = item::none;
        };

        struct separator_meta {};

        struct menu_meta
        {
            std::string caption;

            using item_type = std::variant<menu_item_meta, separator_meta, menu_meta>;
            std::vector<item_type> items;
        };

        std::vector<menu_meta> _structure;
        std::vector<std::pair<ImGuiKey, item>> _hotkey_items;

        void process_menu_meta_frame(const menu_meta& mm);

        void process_structure(std::vector<menu_meta>& initial_data);
        void process_structure_items(std::vector<menu_meta::item_type>& initial_items);
    };
}