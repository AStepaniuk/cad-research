#include "history_base_fixture.h"

class when_redo_single_transaction : public history_base_fixture { };

TEST_F(when_redo_single_transaction, should_not_redo_empty_history)
{
    given_history_is_empty();

    when_redo_performed();

    then_runtime_error_should_be_thrown();
}

TEST_F(when_redo_single_transaction, should_not_redo_whith_no_undone_history)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();

    when_redo_performed();

    then_runtime_error_should_be_thrown();
}

TEST_F(when_redo_single_transaction, should_restore_added_point)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000, 1000});
}

TEST_F(when_redo_single_transaction, should_restore_added_point_2nd_transaction)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({1000, 2000});
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {1000, 1000});
    then_point_should_be(1, {1000, 2000});
}

TEST_F(when_redo_single_transaction, should_restore_added_and_modified_point_2nd_transaction)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_added_to_registry({1000, 2000});
    given_point_is_modified(1, {2000, 3000});
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {1000, 1000});
    then_point_should_be(1, {2000, 3000});
}

TEST_F(when_redo_single_transaction, should_remove_restored_point_2nd_transaction)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_removed_from_registry(0);
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(0);
}

TEST_F(when_redo_single_transaction, should_remove_modified_and_removed_point_2nd_transaction)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_point_is_modified(0, {2000, 3000});
    given_point_removed_from_registry(0);
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(0);
}

TEST_F(when_redo_single_transaction, should_restore_modified_point_2nd_transaction)
{
    given_point_added_to_registry({1000, 5000});
    given_transaction_committed();
    given_point_is_modified(0, {2000, 3000});
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(1);
    then_point_should_be(0, {2000, 3000});
}

TEST_F(when_redo_single_transaction, should_restore_multiple_modifications_2nd_transaction)
{
    given_point_added_to_registry({1000, 5000});
    given_point_added_to_registry({2500, 6000});
    given_transaction_committed();

    given_point_is_modified(0, {2000, 3000});
    given_point_removed_from_registry(1);
    given_point_added_to_registry({3000, 7000});
    given_transaction_committed();
    given_undo_performed();

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {2000, 3000});
    then_point_should_be(2, {3000, 7000});
}

TEST_F(when_redo_single_transaction, should_cancel_uncommitted_added_point)
{
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_undo_performed();
    given_point_added_to_registry({2000, 3000});

    when_redo_performed();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000, 1000});
}

TEST_F(when_redo_single_transaction, should_cancel_uncommitted_removed_point)
{
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_added_to_registry({1000, 1000});
    given_point_added_to_registry({4000, 5000});
    given_transaction_committed();
    given_undo_performed();
    given_point_removed_from_registry(0);

    when_redo_performed();

    then_points_number_should_be(3);
    then_point_should_be(0, {2000, 3000});
    then_point_should_be(1, {1000, 1000});
    then_point_should_be(2, {4000, 5000});
}

TEST_F(when_redo_single_transaction, should_cancel_uncommitted_modified_point)
{
    given_point_added_to_registry({2000, 3000});
    given_transaction_committed();
    given_point_added_to_registry({1000, 1000});
    given_transaction_committed();
    given_undo_performed();
    given_point_is_modified(0, {3000, 4000});

    when_redo_performed();

    then_points_number_should_be(2);
    then_point_should_be(0, {2000, 3000});
    then_point_should_be(1, {1000, 1000});
}
