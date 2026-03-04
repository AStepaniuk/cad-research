#pragma once

#include <concepts>
#include <tuple>
#include <utility>

#include "registry.h"
#include "model_base.h"
#include "one_of.h"
#include "traits.h"

namespace corecad { namespace model
{
    template<typename... TModel>
    requires (std::derived_from<TModel, model_base<TModel>> && ...)
    class registry_pool
    {
    public:
        template <typename T>
        requires IsOneOf<T, TModel...>
        const registry<T>& items() const
        {
            return std::get<registry<T>>(_data);
        }

        template <typename T>
        requires IsOneOf<T, TModel...>
        registry<T>& items()
        {
            return std::get<registry<T>>(_data);
        }

        template <typename T>
        requires IsOneOf<T, TModel...>
        void clear()
        {
            items<T>().clear();
        }

        template <typename T>
        requires IsOneOf<T, TModel...>
        T::index_t put(T val)
        {
            return items<T>().put(std::move(val));
        }

        template<typename U>
        requires (util::ConstructibleIntoExactlyOne<U, TModel...> && !IsOneOf<U, TModel...>)
        util::unique_constructible_t<U, TModel...>::index_t put(U val)
        {
            using T = util::unique_constructible_t<U, TModel...>;
            return put(T { std::move(val) });
        }

        template <typename T, typename... TArgs>
        requires IsOneOf<T, TModel...>
        T::index_t make(TArgs&&... vals)
        {
            return items<T>().make(vals...);
        }

        template <typename TIndex>
        requires IsOneOf<typename TIndex::tag_t, TModel...>
        bool erase(const TIndex& index)
        {
            return items<typename TIndex::tag_t>().erase(index);
        }

        template <typename TIndex>
        requires IsOneOf<typename TIndex::tag_t, TModel...>
        const typename TIndex::tag_t& get(const TIndex& index) const
        {
            return items<typename TIndex::tag_t>().get(index);
        }
 
        template <typename TIndex>
        requires IsOneOf<typename TIndex::tag_t, TModel...>
        typename TIndex::tag_t& get(const TIndex& index)
        {
            return items<typename TIndex::tag_t>().get(index);
        }

        template <typename T>
        size_t size()
        {
            return items<T>().size();
        }

    private:
        std::tuple<registry<TModel>...> _data;
    };
}}
