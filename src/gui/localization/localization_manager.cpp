#include "localization_manager.h"

#include <iostream>

#include "translation_registry.h"

using namespace gui::localization;

std::string_view localization_manager::translate(std::string_view key) const
{
    if (current_lang == language::english) return key;

    const auto& active_dict = get_translations(current_lang);
    auto it = active_dict.find(key);
    if (it != active_dict.end())
    {
        return it->second;
    }
    return key; // Fallback to English version
}

void localization_manager::verify_integrity() const 
{
    const auto& required_keys = translation_registry::get_all_keys();

    for (const auto lang : get_languages())
    {
        if (lang == language::english)
        {
            continue;
        }

        const auto& dict = get_translations(lang);

        for (std::string_view key : required_keys)
        {
            if (dict.find(key) == dict.end())
            {
                std::cout << "Missing translation for key: " << key << std::endl;
            }
        }
    }
}