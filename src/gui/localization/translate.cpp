#include "translate.h"

#include "translation_registry.h"

using namespace gui::localization;

translatable_string::translatable_string(const char *s)
    : key(s)
{
    translation_registry::register_key(key);
}

translatable_string::translatable_string(std::string_view s)
    : key(s)
{
    translation_registry::register_key(key);
}

std::string_view gui::localization::tr(translatable_string fmt)
{
    return localization_manager::instance().translate(fmt.key);
}
