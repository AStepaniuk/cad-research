#pragma once

#include <type_traits>

namespace corecad { namespace model
{
    template <typename TValue, typename TModel>
    class property
    {
        TValue _value;
        TModel* _parent;

        bool updated = false;

    public:
        using value_t = TValue;

        property(TModel& parent, TValue value)
            : _value { std::move(value) }
            , _parent { &parent }
        {}

        property(TModel& parent)
            : _value { }
            , _parent { &parent }
        {}

        property(const property& other)
            : _value { other._value }
            , _parent { nullptr }
        {}

        property(property&& other)
            : _value { std::move(other._value) }
            , _parent { nullptr }
        {}

        property& operator=(const property& other)
        {
            if (this != &other && _value != other._value)
            {
                _value = other._value;
                handle_update();
            }

            return *this;
        }

        property& operator=(property&& other)
        {
            if (this != &other && _value != other._value)
            {
                _value = std::move(other._value);
                handle_update();
            }

            return *this;
        }

        property& operator=(const TValue& other)
        {
            if (_value != other)
            {
                _value = other;
                handle_update();
            }

            return *this;
        }

        property& operator=(TValue&& other)
        {
            if (_value != other)
            {
                _value = std::move(other);
                handle_update();
            }

            return *this;
        }

        operator const TValue& () const { return _value; }
        const TValue& val() const { return _value; }

        auto operator<=>(const property& other) const { return _value <=> other._value; }
        bool operator==(const property& other) const { return _value == other._value; }

        auto operator<=>(const TValue& rhs) const { return _value <=> rhs; }
        bool operator==(const TValue& rhs) const { return _value == rhs; }

        friend auto operator<=>(const TValue& lhs, const property& rhs)
        {
            return lhs <=> rhs._value;
        }

        friend bool operator==(const TValue& lhs, const property& rhs)
        {
            return lhs == rhs._value;
        }

        void reset_updated()
        {
            updated = false;
        }

        void bind(TModel& parent) { _parent = &parent; }

    private:
        void handle_update()
        {
            if (!updated && _parent)
            {
                _parent->notify_updated();
            }

            updated = true;
        }
    };  
      
    template <typename T>
    struct is_property : std::false_type {};

    template <typename TValue, typename TModel>
    struct is_property<property<TValue, TModel>> : std::true_type {};

    template <typename T>
    concept IsProperty = is_property<std::remove_cvref_t<T>>::value;
}}
