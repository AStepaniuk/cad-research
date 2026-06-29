#pragma once

#include <tuple>
#include <functional>

#include <imgui.h>

#include "registry.h"
#include "vector2d.h"
#include "abstract_point.h"
#include "one_of.h"
#include "property.h"
#include "type_list.h"

namespace gui {
    template <typename TList>
    class coord_translator;

    template <corecad::model::IsVector2D... TVector>
    class coord_translator<corecad::util::type_list<TVector...>>
    {
        constexpr static float MinScale = 0.0001;

        template<typename TVec>
        using registry_ref_t = std::reference_wrapper<const corecad::model::registry<TVec>>;

        using vector_list = corecad::util::type_list<TVector...>;
        using vectors_variant = vector_list::variant_t;
        using vectors_indexes_variant = corecad::model::to_index_type_list <vector_list>::type::variant_t;

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
        requires corecad::util::IsOneOf<TVec, TVector...>
        ImVec2 to_view(const TVec& p) const
        {
            return ImVec2
            {
                static_cast<float>((p.x * _x_scale) + _x_offset),
                static_cast<float>((p.y * _y_scale) + _y_offset)
            };
        }

        template<typename TIndex>
        requires (!corecad::model::IsProperty<TIndex> && corecad::util::IsOneOf<typename TIndex::tag_t, TVector...>)
        ImVec2 to_view(TIndex index) const
        {
            constexpr std::size_t idx = corecad::util::index_of<typename TIndex::tag_t, vector_list>::value;

            const auto& registry = std::get<idx>(_points).get();
            return to_view(registry.get(index));
        }

        template <corecad::model::IsProperty TProperty>
        requires (corecad::model::IsRegistryIndex<typename TProperty::value_t>
            && corecad::util::IsOneOf<typename TProperty::value_t::tag_t, TVector...>)
        ImVec2 to_view(TProperty property) const
        {
            const auto index = property.val();
            return to_view(index);
        }

        ImVec2 to_view(const vectors_variant& variant_p) const
        {
            return std::visit(
                [this](const auto& p) { return to_view(p); }
                , variant_p
            ); 
        }

        ImVec2 to_view(const vectors_indexes_variant& variant_index) const
        {
            return std::visit(
                [this](const auto& index) { return to_view(index); }
                , variant_index
            ); 
        }


    private:
        std::tuple<registry_ref_t<TVector>...> _points;

        int _x_offset = 0;
        int _y_offset = 0;

        float _x_scale = 0.1;
        float _y_scale = 0.1;
    };

    // translator template instance used in view
    using translator_t = coord_translator<domain::plan::model::shape::wall_points_tl>;
}
