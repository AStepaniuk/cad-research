#pragma once

#include <variant>

#include "variant_model_base.h"
#include "registry.h"
#include "one_of.h"
#include "type_list.h"
#include "member_info.h"

#include "aligned.h"
#include "fixed.h"
#include "offset.h"
#include "parallel_distant.h"

namespace corecad::model::constraint
{
    template <typename TPoint2DIndexList>
    requires util::AllElementsAre<TPoint2DIndexList, is_point2d_index>
    struct constraint;

    template <typename TPoint2DIndexList>
    using constraint_base = variant_model_base<
        constraint<TPoint2DIndexList>
        , aligned<TPoint2DIndexList, constraint<TPoint2DIndexList>>
        , fixed<TPoint2DIndexList, constraint<TPoint2DIndexList>>
        , offset<TPoint2DIndexList, constraint<TPoint2DIndexList>>
        , parallel_distant<TPoint2DIndexList, constraint<TPoint2DIndexList>>
    >;
    
    template <typename TPoint2DIndexList>
    requires util::AllElementsAre<TPoint2DIndexList, is_point2d_index>
    struct constraint : constraint_base<TPoint2DIndexList>
    {
        using point_id_t = TPoint2DIndexList::variant_t;

        template<template <typename, typename> typename TInst>
        using concrete_t = TInst<TPoint2DIndexList, constraint<TPoint2DIndexList>>;

        struct metadata
        {
            static constexpr std::string_view type_name = "constraint";

            static constexpr auto members = std::make_tuple(
                meta::member("index", &constraint::index),
                meta::member("instance", &constraint::instance)
            );
        };

    protected:
        explicit constraint(typename constraint_base<TPoint2DIndexList>::instance_t i)
            : constraint_base<TPoint2DIndexList> { std::move(i) }
        {
        }

    private:
        friend constraint_base<TPoint2DIndexList>; 
    };

    template <typename TPoint2DIndexList>
    std::ostream& operator<<(std::ostream& os, const constraint<TPoint2DIndexList>& c)
    {
        os << static_cast<const constraint_base<TPoint2DIndexList>&>(c);

        return os;
    }

    template <typename T>
    struct is_constraint : std::false_type {};

    template <typename TPoint2DIndexList>
    struct is_constraint<constraint<TPoint2DIndexList>> : std::true_type {};

    template <typename T>
    concept IsConstraint = is_constraint<std::remove_cvref_t<T>>::value;
}
