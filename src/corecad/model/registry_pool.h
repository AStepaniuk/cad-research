#pragma once

#include <concepts>
#include <tuple>
#include <utility>

#include "registry.h"
#include "model_base.h"
#include "one_of.h"
#include "traits.h"
#include "property.h"
#include "type_meta_info.h"

namespace corecad::model
{
    namespace impl
    {
        template <typename T>
        struct model_type
        {
            using type = T;
        };

        template <typename T1, typename T2>
        struct model_type<std::pair<T1, T2>>
        {
            using type = T1;
        };

        template <typename T>
        using model_type_t = typename model_type<T>::type;


        template <typename T>
        struct annotation_type
        {
            using type = nothing;
        };

        template <typename T1, typename T2>
        struct annotation_type<std::pair<T1, T2>>
        {
            using type = T2;
        };

        template <typename T>
        using annotation_type_t = typename annotation_type<T>::type;


        template <typename T, typename... Args>
        constexpr size_t get_model_index()
        {
            bool matches[] = { std::is_same_v<T, model_type_t<Args>>... };
            for (size_t i = 0; i < sizeof...(Args); ++i)
            {
                if (matches[i]) return i;
            }
            return -1; // Should be unreachable
        }


        template <typename TQueryModel, typename... TModelDataPack>
        struct find_model_data_pack;

        template <typename TQueryModel, typename Head, typename... Tail>
        requires std::is_same_v<TQueryModel, impl::model_type_t<Head>>
        struct find_model_data_pack<TQueryModel, Head, Tail...>
        {
            using data_pack_t = Head;
        };

        template <typename TQueryModel, typename Head, typename... Tail>
        struct find_model_data_pack<TQueryModel, Head, Tail...> 
            : find_model_data_pack<TQueryModel, Tail...> {};
    }

    template<typename... TModelData>
    requires (std::derived_from<impl::model_type_t<TModelData>, model_base<impl::model_type_t<TModelData>>> && ...)
    class registry_pool
    {
    public:
        template <typename TQueryModel>
        using annotation_type_for_t = impl::annotation_type_t<
            typename impl::find_model_data_pack<TQueryModel, TModelData...>::data_pack_t
        >;

        template <typename T>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        const auto& items() const
        {
            constexpr size_t idx = impl::get_model_index<T, TModelData...>();
            return std::get<idx>(_data);
        }

        template <typename T>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        auto& items()
        {
            constexpr size_t idx = impl::get_model_index<T, TModelData...>();
            return std::get<idx>(_data);
        }

        template <typename T>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        void clear()
        {
            items<T>().clear();
        }

        template <typename T>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        typename T::index_t put(T val)
        {
            return items<T>().put(std::move(val));
        }

        template<typename U>
        requires (
            util::ConstructibleIntoExactlyOne<U, impl::model_type_t<TModelData>...>
            && !util::IsOneOf<U, impl::model_type_t<TModelData>...>
        )
        typename util::unique_constructible_t<U, impl::model_type_t<TModelData>...>::index_t put(U val)
        {
            using T = util::unique_constructible_t<U, impl::model_type_t<TModelData>...>;
            return put(T { std::move(val) });
        }

        template <typename T, typename... TArgs>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        T::index_t make(TArgs&&... vals)
        {
            return items<T>().make(vals...);
        }

        template <typename T>
        requires util::IsOneOf<T, impl::model_type_t<TModelData>...>
        void restore(T val)
        {
            return items<T>().restore(std::move(val));
        }

        template <typename TIndex>
        requires util::IsOneOf<typename TIndex::tag_t, impl::model_type_t<TModelData>...>
        bool erase(const TIndex& index)
        {
            return items<typename TIndex::tag_t>().erase(index);
        }

        template <typename TIndex>
        requires (!IsProperty<TIndex> && util::IsOneOf<typename TIndex::tag_t, impl::model_type_t<TModelData>...>)
        const typename TIndex::tag_t& get(const TIndex& index) const
        {
            return items<typename TIndex::tag_t>().get(index);
        }
 
        template <typename TIndex>
        requires (!IsProperty<TIndex> && util::IsOneOf<typename TIndex::tag_t, impl::model_type_t<TModelData>...>)
        typename TIndex::tag_t& get(const TIndex& index)
        {
            return items<typename TIndex::tag_t>().get(index);
        }

        template <IsProperty TProperty>
        requires (IsRegistryIndex<typename TProperty::value_t>
            && util::IsOneOf<typename TProperty::value_t::tag_t, impl::model_type_t<TModelData>...>)
        const typename TProperty::value_t::tag_t& get(const TProperty& index_prop) const
        {
            return items<typename TProperty::value_t::tag_t>().get(index_prop.val());
        }

        template <IsProperty TProperty>
        requires (IsRegistryIndex<typename TProperty::value_t>
            && util::IsOneOf<typename TProperty::value_t::tag_t, impl::model_type_t<TModelData>...>)
        typename TProperty::value_t::tag_t& get(const TProperty& index_prop)
        {
            return items<typename TProperty::value_t::tag_t>().get(index_prop.val());
        }

        template <typename TIndex>
        requires (!IsProperty<TIndex> && util::IsOneOf<typename TIndex::tag_t, impl::model_type_t<TModelData>...>)
        const annotation_type_for_t<typename TIndex::tag_t>& annotation(const TIndex& index) const
        {
            return items<typename TIndex::tag_t>().annotation(index);
        }
 
        template <typename TIndex>
        requires (!IsProperty<TIndex> && util::IsOneOf<typename TIndex::tag_t, impl::model_type_t<TModelData>...>)
        annotation_type_for_t<typename TIndex::tag_t>& annotation(const TIndex& index)
        {
            return items<typename TIndex::tag_t>().annotation(index);
        }

        template <IsProperty TProperty>
        requires (IsRegistryIndex<typename TProperty::value_t>
            && util::IsOneOf<typename TProperty::value_t::tag_t, impl::model_type_t<TModelData>...>)
        const annotation_type_for_t<typename TProperty::value_t::tag_t>& annotation(const TProperty& index_prop) const
        {
            return items<typename TProperty::value_t::tag_t>().annotation(index_prop.val());
        }

        template <IsProperty TProperty>
        requires (IsRegistryIndex<typename TProperty::value_t>
            && util::IsOneOf<typename TProperty::value_t::tag_t, impl::model_type_t<TModelData>...>)
        annotation_type_for_t<typename TProperty::value_t::tag_t>& annotation(const TProperty& index_prop)
        {
            return items<typename TProperty::value_t::tag_t>().annotation(index_prop.val());
        }

        template <typename T>
        size_t size()
        {
            return items<T>().size();
        }

    private:
        std::tuple<
            registry<
                impl::model_type_t<TModelData>,
                impl::annotation_type_t<TModelData>
            >...
        > _data;
    };

    template<typename... TModelData>
    std::ostream& operator<<(std::ostream& os, const registry_pool<TModelData...>& p)
    {
        auto out_model = [&](const auto& registry) {
            using registry_t = std::remove_cvref_t<decltype(registry)>;
            using data_t = typename registry_t::data_t;

            os << meta::type_name<data_t>() << std::endl;
            os << registry;
        };

        (out_model(p.template items<impl::model_type_t<TModelData>>()), ...);

        return os;
    }
}

