#pragma once

#include <type_traits>

#include "trackable_registry.h"
#include "history/model_history.h"

namespace corecad { namespace model
{
    template<typename TModel>
    using registry = trackable_registry<TModel, history::model_history<TModel>>;
}}
