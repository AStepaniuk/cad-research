#pragma once

#include <optional>

namespace gui::ui_controller
{
    template <typename TData>
    class control_trigger
    {
        std::optional<TData> _value = std::nullopt;

    public:
        control_trigger& operator=(TData val)
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