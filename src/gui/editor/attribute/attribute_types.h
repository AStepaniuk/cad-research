#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>

#include "floor.h"

namespace gui::editor::attribute
{
    using model_variant_ptr = std::variant<domain::plan::model::shape::wall*>;
    using attr_variant_val = std::variant<double>;
}
