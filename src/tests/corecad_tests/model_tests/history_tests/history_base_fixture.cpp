#include "history_base_fixture.h"

using namespace corecad::model;
using namespace corecad::model::history;

void history_base_fixture::given_history_is_empty()
{
    // do nothing. Just a method placeholder to make test body better readable
}

void history_base_fixture::given_point_added_to_registry(test_point p)
{
    auto idx = _registry.put(std::move(p));
    _points_indexes.push_back(idx);
}

void history_base_fixture::given_transaction_committed()
{
    _history.commit();
}

void history_base_fixture::given_point_removed_from_registry(size_t p_num)
{
    _registry.erase(_points_indexes[p_num]);
}

void history_base_fixture::given_point_is_modified(size_t p_num, const test_point &p)
{
    auto& point = _registry.get(_points_indexes[p_num]);
    point.x = p.x;
    point.y = p.y;
}

void history_base_fixture::given_undo_performed()
{
    _history.undo();
}

void history_base_fixture::given_redo_performed()
{
    _history.redo();
}

void history_base_fixture::when_transaction_cancelled()
{
    _history.cancel();
}

void history_base_fixture::when_undo_performed()
{
    try
    {
        _history.undo();
    }
    catch(const std::runtime_error& e)
    {
        _exception_thrown = e;
    } 
}

void history_base_fixture::when_redo_performed()
{
    try
    {
        _history.redo();
    }
    catch(const std::runtime_error& e)
    {
        _exception_thrown = e;
    } 
}

void history_base_fixture::then_points_number_should_be(size_t num)
{
    ASSERT_EQ(_registry.items<test_point>().size(), num);
}

void history_base_fixture::then_point_should_be(size_t p_num, const test_point &p)
{
    const auto& actual_p = _registry.get(_points_indexes[p_num]);
    ASSERT_TRUE(are_vectors_equal(actual_p, p));
}

void history_base_fixture::then_runtime_error_should_be_thrown()
{
    ASSERT_TRUE(_exception_thrown.has_value());
}
