#pragma once

#include <type_traits>

#include "trackable_registry.h"
#include "history/model_history.h"

namespace corecad::model
{
    template<typename TModel, typename TUserData = nothing>
    using registry = trackable_registry<
        TModel,
        history::model_history<TModel, TUserData>,
        TUserData
    >;
}
