#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>

namespace gui::localization
{
    enum class language { english, russian };

    const std::vector<language>& get_languages();
    const std::unordered_map<std::string_view, std::string_view>& get_translations(language lang);
}
