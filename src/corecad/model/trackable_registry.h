#pragma once

#include <flat_map>
#include <concepts>
#include <ranges>
#include <algorithm>

#include "registry_index.h"
#include "i_model_update_tracker.h"
#include "traits.h"

namespace corecad::model
{
    struct nothing {};

    template<typename TModel>
    class model_base;

    template<typename T, typename THistory, typename TUserData = nothing>
    class trackable_registry : public i_model_update_tracker<T>
    {
    public:
        using index_t = registry_index_t<T>;
        using data_t = T;

    private:
        using underlying_container_t = std::flat_map<index_t, T>;
        using user_data_container_t = std::conditional_t<
            std::same_as<TUserData, nothing>,
            nothing,
            std::flat_map<index_t, TUserData>
        >;

        static constexpr bool _has_user_data = !std::same_as<TUserData, nothing>;

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

            if constexpr (_has_user_data)
            {
                _user_data.clear();
            }
        }

        index_t put(T val)
        {
            ++_last_index;

            auto res = _data.emplace(_last_index, std::move(val));
            auto& item = (*(res.first)).second;
            item.index = _last_index;
            item.bind(this);

            if constexpr (_has_user_data)
            {
                _user_data.emplace(_last_index, TUserData{});
            }

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

            if constexpr (_has_user_data)
            {
                _user_data.emplace(_last_index, TUserData{});
            }

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

            if constexpr (_has_user_data)
            {
                _user_data.emplace(_last_index, TUserData{});
            }

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

            if constexpr (_has_user_data)
            {
                _user_data.erase(index);
            }

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

        bool empty() const
        {
            return _data.empty();
        }

        void bind(THistory* history)
        {
            _history = history;
        }

        const TUserData& user_data(const index_t& index) const
        {
            if constexpr (_has_user_data)
            {
                return _user_data.at(index);
            }
            else
            {
                static_assert(_has_user_data, "Registry has no user data configured");
            }
        }

        TUserData& user_data(const index_t& index)
        {
            if constexpr (_has_user_data)
            {
                return _user_data.at(index);
            }
            else
            {
                static_assert(_has_user_data, "Registry has no user data configured");
            }
        }
     
    private:
        underlying_container_t _data;
        user_data_container_t _user_data;
        index_t _last_index { 0 };
        THistory* _history = nullptr;

        void notify_updating(const T& model) override
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

    namespace io
    {
        inline int get_ud_index()
        {
            static const int index = std::ios_base::xalloc();
            return index;
        }

        inline std::ostream& show_ud(std::ostream& os)
        {
            os.iword(get_ud_index()) = 1;
            return os;
        }

        inline std::ostream& noshow_ud(std::ostream& os)
        {
            os.iword(get_ud_index()) = 0;
            return os;
        }
    }


    template<typename T, typename THistory, typename TUserData>
    std::ostream& operator<<(std::ostream& os, const trackable_registry<T, THistory, TUserData>& r)
    {
        bool show_user_data_runtime = os.iword(io::get_ud_index()) == 1;
        
        for (const auto& [id, item] : r)
        {
            os << id << ": " << item;

            if constexpr (util::streamable<TUserData>) 
            {
                if (show_user_data_runtime) 
                {
                    os << " // " << r.user_data(id);
                }
            }

            os << std::endl;
        }

        return os;
    }


    template <typename T>
    struct is_trackable_registry : std::false_type {};

    template<typename T, typename THistory, typename TUserData>
    struct is_trackable_registry<trackable_registry<T, THistory, TUserData>> : std::true_type {};

    template <typename T>
    concept IsTrackableRegistry = is_trackable_registry<std::remove_cvref_t<T>>::value;
}
