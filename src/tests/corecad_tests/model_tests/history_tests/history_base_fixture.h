#pragma once

#include <gtest/gtest.h>

#include <vector>
#include <stdexcept>
#include <optional>

#include "history/history.h"
#include "registry_pool.h"
#include "vector2d.h"
#include "vector2d_assertion.h"

class history_base_fixture : public ::testing::Test
{
protected:
    class test_point_tag {};
    using test_point = corecad::model::vector2d<test_point_tag>;

    void given_history_is_empty();
    void given_point_added_to_registry(test_point p);
    void given_transaction_committed();
    void given_point_removed_from_registry(size_t p_num);
    void given_point_is_modified(size_t p_num, const test_point& p);
    void given_undo_performed();
    void given_redo_performed();

    void when_transaction_cancelled();
    void when_undo_performed();
    void when_redo_performed();

    void then_points_number_should_be(size_t num);
    void then_point_should_be(size_t num, const test_point& p);
    void then_runtime_error_should_be_thrown();

    corecad::model::registry_pool<test_point> _registry;
    corecad::model::history::history<test_point> _history { _registry };

    std::vector<test_point::index_t> _points_indexes;
    std::optional<std::runtime_error> _exception_thrown = std::nullopt;
};
