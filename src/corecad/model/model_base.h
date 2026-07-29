#pragma once

#include "registry.h"
#include "registry_index.h"
#include "traits.h"
#include "type_list.h"
#include "members_iterator.h"
#include "property.h"

namespace corecad::model {

    struct nothing {};

    template <typename TModel, typename TUserData = nothing>
    class model_base
    {
    public:
        using model_t = TModel;
        using user_data_t = TUserData;
        using index_t = registry_index_t<TModel>;
        using registry_t = registry<TModel>;
        
        index_t index;
        user_data_t user_data;

        void reset_updated()
        {
            if constexpr (requires { std::declval<TModel>().reset_properties_updated(); })
            {
                static_cast<TModel&>(*this).reset_properties_updated();
            }
            else
            {
                util::visit_members<is_property>(static_cast<TModel&>(*this), [](auto& prop) { prop.reset_updated(); });
            }

            _updated = false;
        }
        
        void bind(registry_t* owner) { _owner = owner; }

    private:
        void notify_updated()
        {
            if (!_updated && _owner)
            {
                _owner->notify_updating(static_cast<TModel&>(*this));
            }

            _updated = true;
        }

        registry_t* _owner = nullptr;
        bool _updated = false;

        // class should have access to notify_updated()
        template<typename TValue, typename TMod>
        requires util::EqualityComparableEx<TValue>
        friend class property;
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const model_base<TModel>& model)
    {
        return os << "id:" << model.index;
    }

    template <typename TList>
    struct to_index_type_list;

    template <typename... Ts>
    requires (std::is_base_of_v<model_base<Ts>, Ts> && ...)
    struct to_index_type_list<util::type_list<Ts...>>
    {
        using type = util::type_list<typename Ts::index_t...>;
    };
}
