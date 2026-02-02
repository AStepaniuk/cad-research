#pragma once

#include <unordered_map>
#include <concepts>

#include "registry_index.h"
#include "model_base.h"

namespace corecad { namespace model
{
    template<typename T>
    requires std::derived_from<T, model_base<T>>
    class registry
    {
    public:
        using index_t = registry_index_t<T>;

    private:
        using underlying_contaier_t = std::unordered_map<index_t, T>;

    public:
        using const_iterator_t = underlying_contaier_t::const_iterator;
        using iterator_t = underlying_contaier_t::iterator;

        void clear()
        {
            _data.clear();
        }

        index_t put(T val)
        {
            ++_last_index;

            auto res = _data.emplace(_last_index, std::move(val));
            (*(res.first)).second.index = _last_index;

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
            (*(res.first)).second.index = _last_index;

            return _last_index;
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
     
    private:
        underlying_contaier_t _data;

        index_t _last_index { 0 };
    };
}}
