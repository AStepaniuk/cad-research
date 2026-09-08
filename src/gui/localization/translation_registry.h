#pragma once

#include <string_view>
#include <vector>

namespace gui::localization
{
    class translation_registry
    {
    public:
        static void register_key(std::string_view key);

        static const std::vector<std::string_view>& get_all_keys();
    };
}
