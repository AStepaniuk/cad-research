#include "history_base_fixture.h"

class when_undo_redo_multiple_transactions : public history_base_fixture { };

TEST_F(when_undo_redo_multiple_transactions, should_undo_two_transactions)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_is_modified(0, {4000, 5000});
    given_transaction_committed();
    given_undo_performed();

    when_undo_performed();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000, 1000});
}

TEST_F(when_undo_redo_multiple_transactions, should_redo_two_transactions)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_is_modified(0, {4000, 5000});
    given_transaction_committed();
    given_undo_performed();
    given_undo_performed();
    given_redo_performed();

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {4000, 5000});
    then_point_should_be(1, {2000, 3000});
}

TEST_F(when_undo_redo_multiple_transactions, should_undo_two_transactions_2nd_time)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_is_modified(0, {4000, 5000});
    given_transaction_committed();
    given_undo_performed();
    given_undo_performed();
    given_redo_performed();
    given_redo_performed();
    given_undo_performed();

    when_undo_performed();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000, 1000});
}

TEST_F(when_undo_redo_multiple_transactions, should_redo_two_transactions_2nd_time)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_is_modified(0, {4000, 5000});
    given_transaction_committed();
    given_undo_performed();
    given_undo_performed();
    given_redo_performed();
    given_redo_performed();
    given_undo_performed();
    given_undo_performed();
    given_redo_performed();

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {4000, 5000});
    then_point_should_be(1, {2000, 3000});
}
