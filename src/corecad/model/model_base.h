#pragma once

#include "registry.h"
#include "registry_index.h"

namespace corecad { namespace model {

    template <typename TModel>
    class model_base
    {
    public:
        using model_t = TModel;
        using index_t = registry_index_t<TModel>;
        using registry_t = registry<TModel>;
        
        index_t index;

        void reset_updated()
        {
            static_cast<TModel&>(*this).reset_properties_updated();
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
        friend class property;
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const model_base<TModel>& model)
    {
        return os << "id:" << model.index;
    }
}}
