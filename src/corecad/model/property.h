#pragma once

#include <iostream>

namespace corecad { namespace model
{
    template <typename TValue, typename TModel>
    class property
    {
        TValue _value;
        TModel* _parent;

        bool updated = false;

    public:
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
}}
