#pragma once

#include "registry_index.h"

namespace corecad { namespace model {

    template <typename TModel>
    class model_base
    {
    public:
        using index_t = registry_index_t<TModel>;
        
        index_t index;
    };
}}
