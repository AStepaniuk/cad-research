#pragma once

#include <type_traits>

template <typename T, typename... U>
concept IsOneOf = (std::is_same_v<T, U> || ...);