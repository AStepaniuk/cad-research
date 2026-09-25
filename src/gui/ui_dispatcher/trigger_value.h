#pragma once

#include <optional>

namespace gui::ui_dispatcher
{
    template <typename TData>
    class trigger_value
    {
        std::optional<TData> _value = std::nullopt;

    public:
        trigger_value& operator=(TData val)
        {
            _value = std::move(val);
            return *this;
        }

        std::optional<TData> take()
        {
            auto res = std::move(_value);
            _value = std::nullopt;

            return res;
        }
    };
}