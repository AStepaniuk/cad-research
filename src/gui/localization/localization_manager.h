#pragma once

#include "translation_registry.h"
#include "translations.h"

namespace gui::localization
{
    class localization_manager
    {
    public:
        static localization_manager& instance()
        {
            static localization_manager inst;
            return inst;
        }

        void set_language(language lang) { current_lang = lang; }

        std::string_view translate(std::string_view key) const;

        void verify_integrity() const;

    private:
        language current_lang = language::english;
    };

    // Standard type-safe runtime lookup wrapper
}
