#include "translations.h"

#include <stdexcept>
#include <ranges>

using namespace gui::localization;

namespace
{
    const std::unordered_map<language, std::unordered_map<std::string_view, std::string_view>>& get_translations_table()
    {
        static std::unordered_map<language, std::unordered_map<std::string_view, std::string_view>> translations
        {
            {
                language::russian,
                std::unordered_map<std::string_view, std::string_view>
                {

                }
            }
        };

        return translations;
    }

    std::vector<language> build_languages_list()
    {
        auto list = std::views::keys(get_translations_table()) | std::ranges::to<std::vector>();
        list.push_back(language::english);

        return list;
    }
}

const std::unordered_map<std::string_view, std::string_view>& gui::localization::get_translations(language lang)
{

    if (lang == language::english)
    {
        throw std::runtime_error("Internal error: English translation is requested");
    }

    const auto& table = get_translations_table();

    const auto lang_it = table.find(lang);
    if (lang_it == get_translations_table().end())
    {
        throw std::runtime_error("Requested language is not supported");
    }

    return lang_it->second;
}

const std::vector<language>& gui::localization::get_languages()
{
    static std::vector instance = build_languages_list();

    return instance;
}
