#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <functional>
#include <stdexcept>
#include <format>
#include <concepts>

#include "attribute_types.h"
#include "type_meta_info.h"

namespace gui::editor::attribute
{
    struct attribute_descriptor
    {
        std::string name;
        std::string short_name;
        std::string localized_name;
        std::string localized_short_name;

        std::function<attr_variant_val(const model_variant_ptr&)> reader;
        std::function<std::optional<std::string>(const model_variant_ptr&, attr_variant_val, const domain::plan::model::floor&)> writer;
    };

    template <typename TModel, typename TMember>
    auto make_getter(TMember TModel::*member_ptr) -> std::function<attr_variant_val(const model_variant_ptr&)> 
    {
        return [member_ptr](const model_variant_ptr& var_ptr) -> attr_variant_val {
            return std::visit([member_ptr](auto* active_ptr) -> attr_variant_val {
                using active_type_t = std::remove_pointer_t<decltype(active_ptr)>;
                if constexpr (std::is_same_v<active_type_t, TModel>)
                {
                    if (active_ptr != nullptr)
                    {
                        return active_ptr->*member_ptr;
                    }
                    throw std::runtime_error("Attempted to read attribute from a null model pointer");
                }
                else 
                {
                    throw std::runtime_error("Type mismatch: model_variant_ptr does not contain the expected model type.");
                }
            }, var_ptr);
        };
    }
    
    template <typename TModel, typename TMember, typename TValidator>
    auto make_writer(TMember TModel::*member_ptr, TValidator validator)
        -> std::function<std::optional<std::string>(const model_variant_ptr&, attr_variant_val, const domain::plan::model::floor&)>
    {
        return
            [member_ptr, validator = std::move(validator)]
            (const model_variant_ptr& var_ptr, attr_variant_val variant_val, const domain::plan::model::floor& f) -> std::optional<std::string> {
                return std::visit([member_ptr, &validator, &f](auto* active_ptr, auto&& unpacked_val) -> std::optional<std::string> {
                    using active_type_t = std::remove_pointer_t<decltype(active_ptr)>;
                    using value_type_t = std::decay_t<decltype(unpacked_val)>;

                    if constexpr (std::is_same_v<active_type_t, TModel>)
                    {
                        if (active_ptr != nullptr)
                        {
                            if constexpr (std::is_assignable_v<TMember&, value_type_t>)
                            {
                                std::optional<std::string> validation_error = validator(*active_ptr, unpacked_val, f);
                                if (validation_error)
                                {
                                    return validation_error;
                                }

                                active_ptr->*member_ptr = std::move(unpacked_val);

                                return std::nullopt;
                            }
                            else
                            {
                                throw std::runtime_error(std::format(
                                    "Attempted to write value of an incompatible type '{}' into an attribute of type '{}'. Model type: '{}'",
                                    corecad::meta::type_name<value_type_t>(),
                                    corecad::meta::type_name<TMember>(),
                                    corecad::meta::type_name<TModel>()
                                ));
                            }
                        }
                        throw std::runtime_error("Attempted to write attribute to a null model pointer");
                    }
                    else 
                    {
                        throw std::runtime_error(std::format(
                            "Type mismatch: model_variant_ptr of type '{}' does not contain the expected model type '{}'.",
                            corecad::meta::type_name<active_type_t>(),
                            corecad::meta::type_name<TModel>()
                        ));
                    }
                }, var_ptr, variant_val);
            };
    };
};
