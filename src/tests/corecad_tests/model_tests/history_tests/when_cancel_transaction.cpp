#include "history_base_fixture.h"

class when_cancel_transaction : public history_base_fixture { };

TEST_F(when_cancel_transaction, should_remove_added_point)
{
    given_point_added_to_registry({1000.0 * mm, 1000.0 * mm});

    when_transaction_cancelled();

    then_points_number_should_be(0);
}

TEST_F(when_cancel_transaction, should_remove_added_point_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 1000.0 * mm});
    given_transaction_committed();
    given_point_added_to_registry({1000.0 * mm, 2000.0 * mm});

    when_transaction_cancelled();

    then_points_number_should_be(1);
}

TEST_F(when_cancel_transaction, should_remove_added_and_modified_point_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 1000.0 * mm});
    given_transaction_committed();
    given_point_added_to_registry({1000.0 * mm, 2000.0 * mm});
    given_point_is_modified(1, {2000.0 * mm, 3000.0 * mm});

    when_transaction_cancelled();

    then_points_number_should_be(1);
}

TEST_F(when_cancel_transaction, should_restore_removed_point_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 1000.0 * mm});
    given_transaction_committed();
    given_point_removed_from_registry(0);

    when_transaction_cancelled();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000.0 * mm, 1000.0 * mm});
}

TEST_F(when_cancel_transaction, should_restore_modified_and_removed_point_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 1000.0 * mm});
    given_transaction_committed();
    given_point_is_modified(0, {2000.0 * mm, 3000.0 * mm});
    given_point_removed_from_registry(0);

    when_transaction_cancelled();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000.0 * mm, 1000.0 * mm});
}

TEST_F(when_cancel_transaction, should_restore_modified_point_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 5000.0 * mm});
    given_transaction_committed();
    given_point_is_modified(0, {2000.0 * mm, 3000.0 * mm});

    when_transaction_cancelled();

    then_points_number_should_be(1);
    then_point_should_be(0, {1000.0 * mm, 5000.0 * mm});
}

TEST_F(when_cancel_transaction, should_restore_multiple_modifications_2nd_transaction)
{
    given_point_added_to_registry({1000.0 * mm, 5000.0 * mm});
    given_point_added_to_registry({2500.0 * mm, 6000.0 * mm});
    given_transaction_committed();

    given_point_is_modified(0, {2000.0 * mm, 3000.0 * mm});
    given_point_removed_from_registry(1);
    given_point_added_to_registry({3000.0 * mm, 7000.0 * mm});

    when_transaction_cancelled();

    then_points_number_should_be(2);
    then_point_should_be(0, {1000.0 * mm, 5000.0 * mm});
    then_point_should_be(1, {2500.0 * mm, 6000.0 * mm});
}
