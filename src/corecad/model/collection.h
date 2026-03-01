#pragma once

#include <vector>
#include <concepts>
#include <ranges>
#include <algorithm>

#include "registry_index.h"
#include "model_base.h"

namespace corecad { namespace model
{
    template<typename T>
    requires std::derived_from<T, model_base<T>>
    class collection
    {
    public:
        using index_t = registry_index_t<T>;

    private:
        using underlying_contaier_t = std::vector<index_t>;

    public:
        using const_iterator_t = underlying_contaier_t::const_iterator;
        using iterator_t = underlying_contaier_t::iterator;

        void clear()
        {
            _data.clear();
        }

        void put(index_t index)
        {
            _data.push_back(index);
        }

        template <std::ranges::viewable_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, index_t>
        void put(R&& indexes)
        {
            std::ranges::copy(indexes, std::back_inserter(_data));
        }

        size_t remove(index_t index)
        {
            return std::erase(_data, index);
        }

        bool contains(index_t index) const
        {
            return std::ranges::contains(_data, index);
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

        bool empty() const
        {
            return _data.empty();
        }

    private:
        underlying_contaier_t _data;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const collection<T>& collection)
    {
        bool first = true;
        for (const auto& index : collection)
        {
            if (!first)
            {
                os << ", ";
            }
            os << index;

            first = false;
        }

        return os;
    }
}}