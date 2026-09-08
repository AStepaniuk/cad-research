#pragma once

#include <format>

#include "localization_manager.h"

namespace gui::localization
{
    struct translatable_string
    {
        std::string_view key;

        translatable_string(const char* s);
        translatable_string(std::string_view s);
    };

    template <typename... Args>
    std::string tr(translatable_string fmt, Args&&... args)
    {
        std::string_view translated_fmt = localization_manager::instance().translate(fmt.key);
        return std::vformat(translated_fmt, std::make_format_args(args...));
    }
}