#pragma once

#include <variant>

#include "model_base.h"
#include "registry.h"
#include "one_of.h"
#include "type_list.h"

#include "aligned.h"
#include "fixed.h"
#include "offset.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct constraint : public model_base<constraint<TVector2DIndexList>>
    {
        using point_id_t = TVector2DIndexList::variant_t;

        using instances_tl = util::type_list<
            aligned<constraint>,
            fixed<constraint>,
            offset<constraint>
        >;

        using instance_t = typename instances_tl::variant_t;
        instance_t instance;

        template<template <typename> typename TConstraint>
        requires util::IsOneOf<TConstraint<constraint>, instances_tl>
        using concrete_t = TConstraint<constraint>;

        template<template <typename> typename TConstraint, typename... TArgs>
        requires util::IsOneOf<TConstraint<constraint>, instances_tl>
        static constraint create(TArgs&&... args)
        {
            constraint res { concrete_t<TConstraint> { std::forward<TArgs>(args)... } };
            res.bind_internal();

            return res;
        }

        constraint(const constraint& other)
            : model_base<constraint<TVector2DIndexList>> { other }
            , instance { other.instance }
        {
            bind_internal();
        }

        constraint(constraint&& other) noexcept
            : model_base<constraint<TVector2DIndexList>> { std::move(other) }
            , instance { std::move(other.instance) }
        {
            bind_internal();
        }

        constraint& operator=(const constraint& other) = default;
        constraint& operator=(constraint&& other) noexcept = default;

        void reset_properties_updated()
        {
           std::visit([this](auto& impl) {
                impl.reset_properties_updated();
            }, instance);
        }

    private:
        explicit constraint(instance_t i)
            : instance { std::move(i) }
        {}
 
        void bind_internal()
        {
           std::visit([this](auto& impl) {
                impl.bind(*this);
            }, instance);
        }
    };

    template <typename TVector2DIndexList>
    std::ostream& operator<<(std::ostream& os, const constraint<TVector2DIndexList>& c)
    {
        os << static_cast<const model_base<constraint<TVector2DIndexList>>&>(c);

        std::visit([&os](const auto& impl) {
            os << ' ' << impl;
        }, c.instance);

        return os;
    }

    template <typename T>
    struct is_constraint : std::false_type {};

    template <typename TVector2DIndexList>
    struct is_constraint<constraint<TVector2DIndexList>> : std::true_type {};

    template <typename T>
    concept IsConstraint = is_constraint<std::remove_cvref_t<T>>::value;
}
