#pragma once

#include <tuple>
#include <functional>

#include <imgui.h>

#include "registry.h"
#include "vector2d.h"
#include "abstract_point.h"
#include "one_of.h"
#include "property.h"

namespace gui {
    template <corecad::model::IsVector2D... TVector>
    class coord_translator
    {
        constexpr static float MinScale = 0.0001;

        template<typename TVec>
        using registry_ref_t = std::reference_wrapper<const corecad::model::registry<TVec>>;

    public:
        coord_translator(const corecad::model::registry<TVector>&... points)
            : _points { std::ref(points)... }
        {
        }

        int x_offset() const { return _x_offset; }
        void x_offset(int val) { _x_offset = val; }

        int y_offset() const { return _y_offset; }
        void y_offset(int val) { _y_offset = val; }

        float x_scale() const { return _x_scale; }
        void x_scale(float val)
        {
            if (val >= MinScale)
            {
                _x_scale = val;
            }
            else
            {
                _x_scale = MinScale;
            }
        }

        float y_scale() const { return _y_scale; }
        void y_scale(float val)
        {
            if (val >= MinScale)
            {
                _y_scale = val;
            }
            else
            {
                _y_scale = MinScale;
            }
        }

        abstract_point to_model(const ImVec2 screen) const
        {
            return
            {
                (static_cast<double>(screen.x) - _x_offset) / _x_scale,
                (static_cast<double>(screen.y) - _y_offset) / _y_scale
            };
        }

        template<typename TVec>
        requires IsOneOf<TVec, TVector...>
        ImVec2 to_view(const TVec& p) const
        {
            return ImVec2
            {
                static_cast<float>((p.x * _x_scale) + _x_offset),
                static_cast<float>((p.y * _y_scale) + _y_offset)
            };
        }

        template<typename TIndex>
        requires (!corecad::model::IsProperty<TIndex> && IsOneOf<typename TIndex::tag_t, TVector...>)
        ImVec2 to_view(TIndex index) const
        {
            const auto& p = std::get<registry_ref_t<typename TIndex::tag_t>>(_points).get().get(index);

            return to_view(p);
        }

        template <corecad::model::IsProperty TProperty>
        requires (corecad::model::IsRegistryIndex<typename TProperty::value_t>
            && IsOneOf<typename TProperty::value_t::tag_t, TVector...>)
        ImVec2 to_view(TProperty property) const
        {
            return to_view(property.val());
        }


    private:
        std::tuple<registry_ref_t<TVector>...> _points;

        int _x_offset = 0;
        int _y_offset = 0;

        float _x_scale = 0.1;
        float _y_scale = 0.1;
    };

    // translator template instance used in view
    using translator_t = coord_translator<
        domain::plan::model::wall_axis_point,
        domain::plan::model::wall_border_point
    >;
}
