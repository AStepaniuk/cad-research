#pragma once

#include <string_view>
#include <vector>
#include <optional>

#include "attribute_types.h"

namespace gui::editor::attribute
{
    struct attr_info
    {
        std::string_view name;
        bool writable;
    };

    class attribute_service
    {
        const domain::plan::model::floor& _floor;

    public:
        attribute_service(const domain::plan::model::floor& f);

        const std::vector<attr_info>& list_attributes(model_variant_ptr ptr) const;

        attr_variant_val get_attribute(model_variant_ptr ptr, std::string_view attr) const;
        std::optional<std::string> set_attribute(model_variant_ptr ptr, std::string_view attr, attr_variant_val val);
    };
}
