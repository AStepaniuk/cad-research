#include "main_menu.h"

#include <iostream>
#include <imgui.h>

#include "overloaded.h"

using namespace gui;

main_menu::main_menu()
{
    _structure = 
    {
        menu_meta
        {
            .caption = "File",
            .items =
            {
                menu_item_meta 
                {
                    .caption = "Open"
                },                
                menu_item_meta 
                {
                    .caption = "Save"
                },
                separator_meta {},
                menu_item_meta 
                {
                    .caption = "Exit"
                }
            }
        },
        menu_meta
        {
            .caption = "Draw",
            .items = 
            {
                menu_item_meta
                {
                    .caption = "Wall",
                    .hotkey = ImGuiKey_W,
                    .item_choice = item::add_wall
                }
            }
        }
    };

    process_structure(_structure);
}

void main_menu::process_frame()
{
    _choosen_item = item::none;

    if (!ImGui::GetIO().WantCaptureKeyboard) 
    {
        for (const auto& hotkey_pair : _hotkey_items)
        {
            if (ImGui::IsKeyPressed(hotkey_pair.first))
            {
                _choosen_item = hotkey_pair.second;
                break;
            }
        } 
    }

    if (ImGui::BeginMainMenuBar())
    {
        _is_mouse_hovering = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        for (const auto& mm : _structure)
        {
            process_menu_meta_frame(mm);
        }

        ImGui::EndMainMenuBar();
    }
}

bool main_menu::is_mouse_hovering() const
{
    return _is_mouse_hovering;
}

gui::main_menu::item gui::main_menu::choosen_item() const
{
    return _choosen_item;
}

void gui::main_menu::process_menu_meta_frame(const menu_meta &mm)
{
    if (ImGui::BeginMenu(mm.caption.c_str()))
    {
        for (const auto& menu_item : mm.items)
        {
            std::visit(corecad::util::overloaded
                {
                    [this] (const menu_item_meta& mim)
                    {
                        if (ImGui::MenuItem(mim.caption.c_str(), mim.hotkey_text.c_str()))
                        {
                            _choosen_item = mim.item_choice;
                        }
                    },
                    [this] (const menu_meta& mm)
                    {
                        process_menu_meta_frame(mm);
                    },
                    [this] (const separator_meta&)
                    {
                        ImGui::Separator();
                    }
                },
                menu_item
            );
        };
        ImGui::EndMenu();
    }
}

void main_menu::process_structure(std::vector<menu_meta> &initial_data)
{
    for (auto& menu_item : initial_data)
    {
        process_structure_items(menu_item.items);
    }
}

void gui::main_menu::process_structure_items(std::vector<menu_meta::item_type>& initial_items)
{
    for (auto& menu_item : initial_items)
    {
        std::visit(corecad::util::overloaded
            {
                [this] (menu_item_meta& mim)
                {
                    if (mim.hotkey)
                    {
                        mim.hotkey_text = ImGui::GetKeyName(mim.hotkey.value());

                        if (mim.item_choice != item::none)
                        {
                            _hotkey_items.push_back({ mim.hotkey.value(), mim.item_choice });
                        }
                    }
                },
                [this] (menu_meta& mm)
                {
                    process_structure_items(mm.items);
                },
                [this] (separator_meta&) { }
            },
            menu_item
        );
    };
}
