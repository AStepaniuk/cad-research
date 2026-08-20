#pragma once

#include "registry.h"
#include "registry_index.h"
#include "traits.h"
#include "type_list.h"
#include "members_iterator.h"
#include "property.h"
#include "i_model_update_tracker.h"

namespace corecad::model
{
    template <typename TModel>
    class model_base
    {
    public:
        using model_t = TModel;
        using index_t = registry_index_t<TModel>;
        
        index_t index;

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
        
        void bind(i_model_update_tracker<TModel>* update_tracker) { _update_tracker = update_tracker; }

    private:
        void notify_updated()
        {
            if (!_updated && _update_tracker)
            {
                _update_tracker->notify_updating(static_cast<TModel&>(*this));
            }

            _updated = true;
        }

        i_model_update_tracker<TModel>* _update_tracker = nullptr;
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
    struct to_index_type_list<util::type_list<Ts...>>
    {
        using type = util::type_list<typename Ts::index_t...>;
    };
}
