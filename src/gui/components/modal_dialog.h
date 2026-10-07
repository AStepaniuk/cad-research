#pragma once

#include <imgui.h>

namespace gui::components
{
    class modal_dialog;

    class modal_dialog_scope_handle
    {
    public:
        modal_dialog_scope_handle() = default;
        modal_dialog_scope_handle(modal_dialog& dialog);
        ~modal_dialog_scope_handle();

        modal_dialog_scope_handle(const modal_dialog_scope_handle&) = delete;
        modal_dialog_scope_handle& operator=(const modal_dialog_scope_handle&) = delete;
        modal_dialog_scope_handle(modal_dialog_scope_handle&& other) noexcept;
        modal_dialog_scope_handle& operator=(modal_dialog_scope_handle&& other) noexcept;

        explicit operator bool() const;

    private:
        modal_dialog* _dialog = nullptr;
    };

    class modal_dialog
    {
    public:
        modal_dialog(ImVec2 default_size, ImVec2 min_size, ImVec2 max_size);
    
        void open();
        void close();

        modal_dialog_scope_handle begin(const char* title, ImGuiWindowFlags flags = 0);
        void end();

        bool is_active() const;
        bool is_open() const;

    private:
        bool _should_open_popup = false;
        bool _is_active = false;
        bool _is_open = false;

        ImVec2 _default_size;
        ImVec2 _min_size;
        ImVec2 _max_size;
    };
}