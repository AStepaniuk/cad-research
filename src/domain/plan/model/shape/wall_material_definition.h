#pragma once

#include <string>

#include "model_base.h"
#include "member_info.h"

namespace domain::plan::model::shape
{
    class wall_material_definition : public corecad::model::model_base<wall_material_definition>
    {
    public:
        template <typename TValue>
        using property = corecad::model::property<TValue, wall_material_definition>;

        property<std::string> standard_name;
        property<std::string> trade_name;

        property<double> thermal_conductivity; // W/(m·K)
        property<double> density;              // kg/m³

        wall_material_definition()
            : standard_name { this, {} }
            , trade_name { this, {} }
            , thermal_conductivity { this, {} }
            , density { this, {} }
        {}

        wall_material_definition(const wall_material_definition& other)
            : corecad::model::model_base<wall_material_definition> { other }
            , standard_name { this, other.standard_name }
            , trade_name { this, other.trade_name }
            , thermal_conductivity { this, other.thermal_conductivity }
            , density { this, other.density }
        {}

        wall_material_definition(wall_material_definition&& other) noexcept
            : corecad::model::model_base<wall_material_definition> { other }
            , standard_name { this, std::move(other.standard_name) }
            , trade_name { this, std::move(other.trade_name) }
            , thermal_conductivity { this, other.thermal_conductivity }
            , density { this, other.density }
        {}

        wall_material_definition& operator=(const wall_material_definition&) = default;
        wall_material_definition& operator=(wall_material_definition&&) noexcept = default;

        struct metadata
        {
            static constexpr std::string_view type_name = "wall_material_definition"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("index", &wall_material_definition::index),
                corecad::meta::member("standard_name", &wall_material_definition::standard_name),
                corecad::meta::member("trade_name", &wall_material_definition::trade_name),
                corecad::meta::member("thermal_conductivity", &wall_material_definition::thermal_conductivity),
                corecad::meta::member("density", &wall_material_definition::density)
            );
        };
    };
}
