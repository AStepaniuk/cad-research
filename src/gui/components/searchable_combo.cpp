#include "searchable_combo.h"

#include <algorithm>

bool gui::components::impl::contains_case_insensitive(const std::string &haystack, const std::string &needle)
{
    if (needle.empty()) return true;
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    );
    return it != haystack.end();
}
