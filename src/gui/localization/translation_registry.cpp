#include "translation_registry.h"

#include <algorithm>
#include <vector>

using namespace gui::localization;

namespace
{
    std::vector<std::string_view>& get_keys()
    {
        static std::vector<std::string_view> instance;
        return instance;
    }
}

void translation_registry::register_key(std::string_view key)
{
    auto& keys = get_keys();

    if (std::find(keys.begin(), keys.end(), key) == keys.end())
    {
        keys.push_back(key);
    }
}

const std::vector<std::string_view> &translation_registry::get_all_keys()
{
    return get_keys();
}
