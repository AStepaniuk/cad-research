#pragma once

#include <vector>
#include <ranges>
#include <iostream>

#include "std_ostream_extensions.h"

namespace corecad::model
{
    template <typename TValue, typename TModel>
    class list
    {
        using underlying_container_t = std::vector<TValue>;

        underlying_container_t _data;
        TModel* _parent;

        bool _updated = false;

    public:
        using value_t = TValue;
        using const_iterator_t = underlying_container_t::const_iterator;
        using iterator_t = const_iterator_t;

        list(TModel* parent)
            : _data { }
            , _parent { parent }
        {}

        list(const list& other)
            : _data { other._data }
            , _parent { nullptr }
        {}

        list(list&& other) noexcept
            : _data { std::move(other._data) }
            , _parent { nullptr }
        {}

        list& operator=(const list& other)
        {
            if (this != &other && _data != other._data)
            {
                // update notification should happen while _data remains unchanged
                handle_update();
                _data = other._data;
            }

            return *this;
        }

        list& operator=(list&& other) noexcept
        {
            if (this != &other)
            {
                // update notification should happen while _data remains unchanged
                handle_update();
                _data = std::move(other._data);
            }

            return *this;
        }

        template <std::ranges::viewable_range R>
        requires std::constructible_from<TValue, std::ranges::range_value_t<R>>
        list& operator=(const R& other)
        {
            if (!std::ranges::equal(_data, other))
            {
                // update notification should happen while _data remains unchanged
                handle_update();

                _data.assign(other.begin(), other.end());
            }

            return *this;
        }

        list& operator=(std::vector<TValue>&& other) noexcept
        {
            if (_data != other)
            {
                // update notification should happen while _data remains unchanged
                handle_update();
                _data = std::move(other);
            }

            return *this;
        }

        void clear()
        {
            if (!_data.empty())
            {
                handle_update();
                _data.clear();
            }
        }

        void put(TValue val)
        {
            handle_update();
            _data.push_back(std::move(val));
        }

        template <std::ranges::viewable_range R>
        requires std::constructible_from<TValue, std::ranges::range_value_t<R>>
        void put(R&& values)
        {
            if (std::ranges::empty(values)) return;
            
            handle_update();

            if constexpr (std::ranges::sized_range<R>)
            {
                _data.reserve(_data.size() + std::ranges::size(values));
            }
            std::ranges::copy(values, std::back_inserter(_data));
        }

        size_t remove(const TValue& val)
        {
            size_t removed_count = 0;
            auto it = std::ranges::find(_data, val);

            while (it != _data.end())
            {
                if (removed_count == 0)
                {
                    handle_update();
                }

                it = _data.erase(it);
                removed_count++;

                it = std::find(it, _data.end(), val);
            }

            return removed_count;
        }

        const_iterator_t remove(const_iterator_t it)
        {
            handle_update();
            return const_iterator_t { _data.erase(it) }; 
        }

        const_iterator_t remove(const_iterator_t first, const_iterator_t last)
        {
            if (first != last)
            {
                handle_update();
                return const_iterator_t { _data.erase(first, last) };
            }
            return first;
        }

        template <typename Compare>
        void sort(Compare comp)
        {
            if (!std::ranges::is_sorted(_data, comp))
            {
                handle_update();
                std::ranges::sort(_data, comp);
            }
        }

        template <typename Predicate, typename Mutator>
        void update_if(Predicate pred, Mutator mutate)
        {
            bool changed = false;
            for (auto& item : _data)
            {
                if (pred(item))
                {
                    if (!changed)
                    {
                        handle_update();
                        changed = true;
                    }
                    mutate(item);
                }
            }
        }

        bool contains(const TValue& val) const
        {
            return std::ranges::contains(_data, val);
        }

        const_iterator_t begin() const
        {
            return _data.cbegin();
        }

        const_iterator_t end() const
        {
            return _data.cend();
        }

        size_t size() const
        {
            return _data.size();
        }

        bool empty() const
        {
            return _data.empty();
        }

        void reset_updated() { _updated = false; }
        void bind(TModel& parent) { _parent = &parent; }

        struct metadata
        {
            static constexpr std::string_view base_type_name = "list_of_";
            static constexpr std::string_view value_name = corecad::meta::type_name<TValue>();
            static constexpr std::string_view type_name = corecad::util::join_v<base_type_name, value_name>;
        };

    friend std::ostream& operator<<(std::ostream& os, const list& l)
    {
        return os << l._data;
    }

    private:
        void handle_update()
        {
            if (!_updated && _parent)
            {
                _parent->notify_updated();
            }

            _updated = true;
        }
    };  
      
    template <typename T>
    struct is_list : std::false_type {};

    template <typename TValue, typename TModel>
    struct is_list<list<TValue, TModel>> : std::true_type {};

    template <typename T>
    concept IsList = is_list<std::remove_cvref_t<T>>::value;
}
