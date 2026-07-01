#pragma once

#include "iostream"

#include "type_list.h"

namespace corecad::model {
    template <typename TImpl, typename TBase>
    concept VariantModelInstance = requires(TImpl& impl, TBase& base)
    {
        impl.bind(base);
        impl.reset_properties_updated();
    };

    template <typename TVariantModel, typename... TInstance>
    struct variant_model_base : public model_base<TVariantModel>
    {
        using instances_tl = util::type_list<TInstance...>;

        using instance_t = typename instances_tl::variant_t;
        instance_t instance;

        template<template <typename...> typename TInst, typename... TArgs>
        static TVariantModel create(TArgs&&... args)
        {
            TVariantModel res { instance_t { typename TVariantModel::concrete_t<TInst> { std::forward<TArgs>(args)... } } };
            res.bind_internal();

            return res;
        }

        variant_model_base(const variant_model_base& other)
            : model_base<TVariantModel> { other }
            , instance { other.instance }
        {
            bind_internal();
        }

        variant_model_base(variant_model_base&& other) noexcept
            : model_base<TVariantModel>{ std::move(other) }
            , instance{ std::move(other.instance) }
        {
            bind_internal();
        }

        variant_model_base& operator=(const variant_model_base& other)
        {
            if (this != &other)
            {
                model_base<TVariantModel>::operator=(other);
                instance = other.instance;
                bind_internal();
            }

            return *this;
        }

        variant_model_base& operator=(variant_model_base&& other) noexcept
        {
            if (this != &other)
            {
                model_base<TVariantModel>::operator=(std::move(other));
                instance = std::move(other.instance);
                bind_internal();
            }
            return *this;
        }

        void reset_properties_updated()
        {
           std::visit([this](auto& impl) {
                impl.reset_properties_updated();
            }, instance);
        }

    protected:
        explicit variant_model_base(instance_t i)
            : instance { std::move(i) }
        {}

    private:
        void bind_internal()
        {
           std::visit([this](auto& impl) {
                impl.bind(static_cast<TVariantModel&>(*this));
            }, instance);
        }
    };

    template <typename TVariantModel, typename... TInstance>
    std::ostream& operator<<(std::ostream& os, const variant_model_base<TVariantModel, TInstance...>& vm)
    {
        os << static_cast<const model_base<TVariantModel>&>(vm);

        std::visit([&os](const auto& impl) {
            os << ' ' << impl;
        }, vm.instance);

        return os;
    }
}
