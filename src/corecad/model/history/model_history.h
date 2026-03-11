#pragma once

#include <vector>
#include <ranges>

#include "trackable_registry.h"

namespace corecad { namespace model { namespace history
{
    template<typename TModel>
    struct transaction_data
    {
        std::vector<TModel> pre_modified_items;
        std::vector<TModel> post_modified_items;
        std::vector<TModel> added_items;
        std::vector<TModel> deleted_items;
    };
    
    template<typename TModel>
    class model_history
    {
    public:
        using registry_t = trackable_registry<TModel, model_history>;

        void track(registry_t* registry)
        {
            _registry = registry;
            _registry->bind(this);
        }

        void start_transaction(transaction_data<TModel>* transaction)
        {
            if (_current_transaction)
            {
                // if there is existing transaction - gather post-updated item first
                _current_transaction->post_modified_items =
                    _current_transaction->pre_modified_items
                    | std::views::transform([this](const TModel& pre_modified) {
                        return _registry->get(pre_modified.index);
                    })
                    | std::ranges::to<std::vector>();

                // refresh added items as they could be modified after adding
                for (auto& m : _current_transaction->added_items)
                {
                    m = _registry->get(m.index);
                }
            }

            _current_transaction = transaction;
            reset_updated();
        }

        void cancel_transaction()
        {
            if (_current_transaction)
            {
                do_undo_transaction(_current_transaction);
            }
        }

        void undo_transaction(const transaction_data<TModel>* transaction)
        {
            if (_current_transaction)
            {
                do_undo_transaction(transaction);
            }
        }

        void redo_transaction(const transaction_data<TModel>* transaction)
        {
            if (_current_transaction)
            {
                do_redo_transaction(transaction);
            }
        }

        void item_updating(const TModel& model)
        {
            if (_current_transaction)
            {
                auto it = std::ranges::find_if(
                    _current_transaction->added_items, [&model](const TModel& m) {
                        return m.index == model.index;
                    }
                );

                // don't track item as modified if it was added in the current transaction
                if (it == _current_transaction->added_items.end())
                {
                    _current_transaction->pre_modified_items.push_back(model);
                }
            }
        }

        void item_created(const TModel& model)
        {
            if (_current_transaction)
            {
                _current_transaction->added_items.push_back(model);
            }
        }

        void item_deleting(const TModel& model)
        {
            if (_current_transaction)
            {
                // check if the item was added in the current transaction
                auto added_it = std::ranges::find_if(
                    _current_transaction->added_items, [&model](const TModel& m) {
                        return m.index == model.index;
                    }
                );

                if (added_it != _current_transaction->added_items.end())
                {
                    _current_transaction->added_items.erase(added_it);       
                    return;
                }
                
                // check if item was modified before deleting
                auto modified_it = std::ranges::find_if(
                    _current_transaction->pre_modified_items, [&model](const TModel& m) {
                        return m.index == model.index;
                    }
                );

                if (modified_it == _current_transaction->pre_modified_items.end())
                {
                    _current_transaction->deleted_items.push_back(model);
                }
                else
                {
                    // move original item from modified to deleted
                    _current_transaction->deleted_items.push_back(*modified_it);
                    _current_transaction->pre_modified_items.erase(modified_it);
                }
            }
        }

    private:
        registry_t* _registry;
        transaction_data<TModel>* _current_transaction;

        void reset_updated()
        {
            for (auto& m : *_registry)
            {
                m.second.reset_updated();
            }
        }

        void do_undo_transaction(const transaction_data<TModel>* transaction)
        {
            for (const auto& m : transaction->added_items)
            {
                _registry->erase(m.index);
            }

            for (const auto& m : transaction->pre_modified_items)
            {
                _registry->get(m.index) = m;
            }

            for (const auto& m : transaction->deleted_items)
            {
                _registry->restore(m);
            }
        }

        void do_redo_transaction(const transaction_data<TModel>* transaction)
        {
            for (const auto& m : transaction->added_items)
            {
                _registry->restore(m);
            }

            for (const auto& m : transaction->post_modified_items)
            {
                _registry->get(m.index) = m;
            }

            for (const auto& m : transaction->deleted_items)
            {
                _registry->erase(m.index);
            }
        }
    };
}}}
