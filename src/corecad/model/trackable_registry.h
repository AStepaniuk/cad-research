#pragma once

#include <unordered_map>
#include <concepts>
#include <ranges>
#include <algorithm>

#include "registry_index.h"

namespace corecad { namespace model
{
    template<typename TModel>
    class model_base;

    template<typename T, typename THistory>
    class trackable_registry
    {
    public:
        using index_t = registry_index_t<T>;

    private:
        using underlying_container_t = std::unordered_map<index_t, T>;

    public:
        using const_iterator_t = underlying_container_t::const_iterator;
        using iterator_t = underlying_container_t::iterator;

        void clear()
        {
            for (const auto& p : _data)
            {
                notify_deleting(p.second);
            }

            _data.clear();
        }

        index_t put(T val)
        {
            ++_last_index;

            auto res = _data.emplace(_last_index, std::move(val));
            auto& item = (*(res.first)).second;
            item.index = _last_index;
            item.bind(this);

            notify_created(item);

            return _last_index;
        }

        template<typename U>
        requires std::constructible_from<T, U> && (!std::same_as<std::remove_cvref_t<U>, T>)
        index_t put(U val)
        {
            return put(T { std::move(val) });
        }

        template <typename... TArgs>
        index_t make(TArgs&&... vals)
        {
            ++_last_index;

            auto res = _data.emplace(_last_index, T { std::forward<TArgs>(vals)...} );
            auto& item = (*(res.first)).second;
            item.index = _last_index;
            item.bind(this);

            notify_created(item);

            return _last_index;
        }

        void restore(T val)
        {
            const auto existing_it = _data.find(val.index);
            if (existing_it != _data.end())
            {
                throw std::runtime_error("Could not restore item. Given index already exists");
            }

            auto res = _data.emplace(val.index, std::move(val));
            auto& item = (*(res.first)).second;
            item.bind(this);

            notify_created(item);
        }

        bool erase(const index_t& index)
        {
            const auto it = _data.find(index);
            if (it == _data.end())
            {
                return false;
            }

            notify_deleting(it->second);
            _data.erase(it);

            return  true;
        }

        const T& get(const index_t& index) const
        {
            return _data.at(index);
        }

        T& get(const index_t& index)
        {
            return _data.at(index);
        }

        const_iterator_t begin() const
        {
            return _data.cbegin();
        }

        const_iterator_t end() const
        {
            return _data.cend();
        }

        iterator_t begin()
        {
            return _data.begin();
        }

        iterator_t end()
        {
            return _data.end();
        }

        size_t size() const
        {
            return _data.size();
        }

        void bind(THistory* history)
        {
            _history = history;
        }

        friend class model_base<T>;
     
    private:
        underlying_container_t _data;
        index_t _last_index { 0 };
        THistory* _history = nullptr;

        void notify_updating(const T& model)
        {
            if (_history) { _history->item_updating(model); }
        }

        void notify_created(const T& model)
        {
            if (_history) { _history->item_created(model); }
        }

        void notify_deleting(const T& model)
        {
            if (_history) { _history->item_deleting(model); }
        }
    };

    template<typename T, typename THistory>
    std::ostream& operator<<(std::ostream& os, const trackable_registry<T, THistory>& r)
    {
        os << "items:" << std::endl;

        auto pointers = r 
            | std::views::transform([](const auto& pair) { return &pair; })
            | std::ranges::to<std::vector>();

        std::ranges::sort(pointers, [](const auto* a, const auto* b) {
            return a->first < b->first;
        });

        for (const auto* pair : pointers)
        {
            std::cout << pair->first << ": " << pair->second << std::endl;
        }

        return os;
    }
}}
