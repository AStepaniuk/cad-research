#pragma once

#include <type_traits>
#include <tuple>
#include <utility>
#include <vector>
#include <iostream>

#include "model_base.h"
#include "registry_pool.h"
#include "model_history.h"

namespace corecad { namespace model { namespace history
{
    template<typename... TModel>
    requires (std::derived_from<TModel, model_base<TModel>> && ...)
    class history
    {
    public:
        using registry_pool_t = registry_pool<TModel...>;

        history(registry_pool_t& registry_pool)
            : _registry_pool { registry_pool }
        {
            start_track_registries();
            start_new_transaction();
        }

        // rolls back changes made in the current transaction
        void cancel()
        {
            suspend_tracking();

            revert_uncommitted_changes();
            start_new_transaction();

            resume_tracking();
        }

        // fixes current transaction and opens new one
        void commit()
        {
            if (_current_transaction_index != no_transaction_index)
            {
                _transactions.erase(_transactions.begin() + _current_transaction_index + 1, _transactions.end());
            }

            finish_transaction();

            _transactions.push_back(std::move(_current_transaction));
            _current_transaction_index = _transactions.size() - 1;

            start_new_transaction();
        }

        // rolls back last fixed transaction
        void undo()
        {
            if (_current_transaction_index == no_transaction_index)
            {
                throw std::runtime_error("Could not perform UNDO. Transaction stack is empty");
            }

            suspend_tracking();

            // ensure current transaction is cancelled
            revert_uncommitted_changes();

            revert_transaction_changes(_transactions[_current_transaction_index]);

            _current_transaction_index--;
            start_new_transaction();

            resume_tracking();
        }

        // restores next fixed transaction
        void redo()
        {
            if (_current_transaction_index + 1 >= _transactions.size())
            {
                throw std::runtime_error("Could not perform REDO. No undone transactions left");
            }

            suspend_tracking();

            revert_uncommitted_changes();

            _current_transaction_index++;
            restore_transaction_changes(_transactions[_current_transaction_index]);

            start_new_transaction();

            resume_tracking();
        }
    
    private:
        registry_pool_t& _registry_pool;

        std::tuple<model_history<TModel>...> _model_histories;

        using transaction_t = std::tuple<transaction_data<TModel>...>;
        std::vector<transaction_t> _transactions;
        transaction_t _current_transaction;

        size_t _next_index = 0;

        static constexpr size_t no_transaction_index = static_cast<size_t>(-1);
        size_t _current_transaction_index = no_transaction_index;

        void start_track_registries()
        {
            std::apply([this](auto&&... args) {
                ((track_registry(args)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void track_registry(model_history<TMod>& model_history)
        {
            auto& registry = _registry_pool.template items<TMod>();
            auto& data = std::get<transaction_data<TMod>>(_current_transaction);
            model_history.track(&registry, &data);
        }

        void suspend_tracking()
        {
            std::apply([this](auto&&... args) {
                ((suspend_tracking(args)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void suspend_tracking(model_history<TMod>& model_history)
        {
            model_history.suspend_tracking();
        }

        void resume_tracking()
        {
            std::apply([this](auto&&... args) {
                ((resume_tracking(args)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void resume_tracking(model_history<TMod>& model_history)
        {
            model_history.resume_tracking();
        }

        void start_new_transaction()
        {
            _current_transaction = {};
            _next_index++;
        }

        void finish_transaction()
        {
            std::apply([this](auto&&... args) {
                ((finish_transaction(args)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void finish_transaction(model_history<TMod>& model_history)
        {
            auto& data = std::get<transaction_data<TMod>>(_current_transaction);
            model_history.finish_transaction();
        }

        void revert_uncommitted_changes()
        {
            std::apply([this](auto&&... args) {
                ((revert_uncommitted_changes(args)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void revert_uncommitted_changes(model_history<TMod>& model_history)
        {
            model_history.revert_current_transaction();
        }

        void revert_transaction_changes(transaction_t& transaction)
        {
            std::apply([this, &transaction](auto&&... args) {
                ((revert_transaction_changes(args, transaction)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void revert_transaction_changes(model_history<TMod>& model_history, transaction_t& transaction)
        {
            const auto& data = std::get<transaction_data<TMod>>(transaction);
            model_history.undo_transaction(&data);
        }

        void restore_transaction_changes(transaction_t& transaction)
        {
            std::apply([this, &transaction](auto&&... args) {
                ((restore_transaction_changes(args, transaction)), ...);
            }, _model_histories);
        }

        template<typename TMod>
        void restore_transaction_changes(model_history<TMod>& model_history, transaction_t& transaction)
        {
            const auto& data = std::get<transaction_data<TMod>>(transaction);
            model_history.redo_transaction(&data);
        }
    };
}}}
