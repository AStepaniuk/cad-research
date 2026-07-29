#pragma once

#include <variant>

#include "variant_model_base.h"
#include "registry.h"
#include "one_of.h"
#include "type_list.h"

#include "aligned.h"
#include "fixed.h"
#include "offset.h"
#include "parallel_distant.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList, typename TUserData = nothing>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct constraint;

    template <typename TVector2DIndexList, typename TUserData>
    using constraint_base = variant_model_base<
        constraint<TVector2DIndexList, TUserData>
        , TUserData
        , aligned<TVector2DIndexList, constraint<TVector2DIndexList, TUserData>>
        , fixed<TVector2DIndexList, constraint<TVector2DIndexList, TUserData>>
        , offset<TVector2DIndexList, constraint<TVector2DIndexList, TUserData>>
        , parallel_distant<TVector2DIndexList, constraint<TVector2DIndexList, TUserData>>
    >;
    
    template <typename TVector2DIndexList, typename TUserData>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct constraint : constraint_base<TVector2DIndexList, TUserData>
    {
        using point_id_t = TVector2DIndexList::variant_t;

        template<template <typename, typename> typename TInst>
        using concrete_t = TInst<TVector2DIndexList, constraint<TVector2DIndexList, TUserData>>;

    protected:
        explicit constraint(typename constraint_base<TVector2DIndexList, TUserData>::instance_t i)
            : constraint_base<TVector2DIndexList, TUserData> { std::move(i) }
        {
        }

    private:
        friend constraint_base<TVector2DIndexList, TUserData>; 
    };

    template <typename TVector2DIndexList, typename TUserData>
    std::ostream& operator<<(std::ostream& os, const constraint<TVector2DIndexList, TUserData>& c)
    {
        os << static_cast<const constraint_base<TVector2DIndexList, TUserData>&>(c);

        return os;
    }

    template <typename T>
    struct is_constraint : std::false_type {};

    template <typename TVector2DIndexList, typename TUserData>
    struct is_constraint<constraint<TVector2DIndexList, TUserData>> : std::true_type {};

    template <typename T>
    concept IsConstraint = is_constraint<std::remove_cvref_t<T>>::value;
}
