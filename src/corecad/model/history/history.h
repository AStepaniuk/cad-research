#pragma once

#include <type_traits>
#include <tuple>
#include <utility>
#include <vector>

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
            std::apply([this](auto&&... args) {
                ((track_registry(args)), ...);
            }, _model_histories);

            std::apply([this](auto&&... args) {
                ((update_transaction(args)), ...);
            }, _model_histories);
        }

        // rolls back changes made in the current transaction
        void cancel()
        {
            std::apply([this](auto&&... args) {
                ((cancel_transaction(args)), ...);
            }, _model_histories);

            _current_transaction = {};
        }

        // fixes current transaction and opens new one
        void commit()
        {
            if (_last_actual_transaction != no_transaction_index)
            {
                _transactions.erase(_transactions.begin() + _last_actual_transaction, _transactions.end());
            }

            _transactions.push_back(std::move(_current_transaction));
            _last_actual_transaction = _transactions.size() - 1;

            _current_transaction = {};

            std::apply([this](auto&&... args) {
                ((update_transaction(args)), ...);
            }, _model_histories);
        }

        // rolls back last fixed transaction
        void undo()
        {
            // ensure current transaction is cancelled
            cancel();

            if (_last_actual_transaction != no_transaction_index)
            {
                std::apply([this](auto&&... args) {
                    ((revert_transaction(args, _transactions[_last_actual_transaction])), ...);
                }, _model_histories);
            }

            _last_actual_transaction--;
        }

        // restores next fixed transaction
        void redo()
        {
            // ensure current transaction is cancelled
            cancel();

            if (_last_actual_transaction + 1 < _transactions.size())
            {
                _last_actual_transaction++;

                std::apply([this](auto&&... args) {
                    ((restore_transaction(args, _transactions[_last_actual_transaction])), ...);
                }, _model_histories);
            }
        }
    
    private:
        registry_pool_t& _registry_pool;

        std::tuple<model_history<TModel>...> _model_histories;

        using transaction_t = std::tuple<transaction_data<TModel>...>;
        std::vector<transaction_t> _transactions;
        transaction_t _current_transaction;

        static constexpr size_t no_transaction_index = static_cast<size_t>(-1);
        size_t _last_actual_transaction = no_transaction_index;

        template<typename TMod>
        void track_registry(model_history<TMod>& model_history)
        {
            auto& registry = _registry_pool.template items<TMod>();
            model_history.track(&registry);
        }

        template<typename TMod>
        void update_transaction(model_history<TMod>& model_history)
        {
            auto& data = std::get<transaction_data<TMod>>(_current_transaction);
            model_history.start_transaction(&data);
        }

        template<typename TMod>
        void cancel_transaction(model_history<TMod>& model_history)
        {
            model_history.cancel_transaction();
        }

        template<typename TMod>
        void revert_transaction(model_history<TMod>& model_history, transaction_t& transaction)
        {
            const auto& data = std::get<transaction_data<TMod>>(transaction);
            model_history.undo_transaction(*data);
        }

        template<typename TMod>
        void restore_transaction(model_history<TMod>& model_history, transaction_t& transaction)
        {
            const auto& data = std::get<transaction_data<TMod>>(transaction);
            model_history.redo_transaction(*data);
        }
    };
}}}
