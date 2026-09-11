#include "attribute_service.h"

#include <typeindex>
#include <format>

#include "attribute_descriptor.h"
#include "translate.h"
#include "type_meta_info.h"

using namespace gui::editor::attribute;
using namespace gui::localization;
using namespace domain::plan::model::shape;
using namespace domain::plan::model;

namespace
{
    std::vector<attribute_descriptor>& get_wall_attribute_data()
    {
        static std::vector<attribute_descriptor> instance {
            attribute_descriptor {
                .name = "thickness",
                .short_name = "t",

                .reader = make_getter(&wall::thickness),
                .writer = make_writer(
                    &wall::thickness,
                    [](const wall& _, double new_thickness, const floor& __) -> std::optional<std::string> {
                        if (new_thickness > 0.0)
                        {
                            return std::nullopt;
                        }
                        else
                        {
                            return tr("Wall thickness should be greater than zero");
                        }
                    }
                )
            }
        };

        return instance;
    }

    std::unordered_map<std::type_index, std::vector<attribute_descriptor>&>& get_all_attribute_data()
    {
        static std::unordered_map<std::type_index, std::vector<attribute_descriptor>&> instance {
            { typeid(wall), get_wall_attribute_data() }
        };

        return instance;
    }

    attr_info to_attr_info(const attribute_descriptor& ad)
    {
        return attr_info {
            .name = ad.name,
            .writable = static_cast<bool>(ad.writer)
        };
    }

    std::unordered_map<std::type_index, std::vector<attr_info>> build_all_attr_info()
    {
        return get_all_attribute_data() 
            | std::views::transform([](const auto& pair) {
                return std::make_pair(
                    pair.first,
                    pair.second 
                        | std::views::transform([](const attribute_descriptor& ad) { return to_attr_info(ad); }) 
                        | std::ranges::to<std::vector<attr_info>>()
                    );
            })
            | std::ranges::to<std::unordered_map<std::type_index, std::vector<attr_info>>>();

    }

    std::type_index get_type_index(const model_variant_ptr &ptr)
    {
        return std::visit([](auto* active_ptr) -> std::type_index {
            using raw_type = std::remove_pointer_t<decltype(active_ptr)>;
            return typeid(raw_type);
        }, ptr);
    }

    std::string_view get_type_name(const model_variant_ptr &ptr)
    {
        return std::visit([](auto* active_ptr) -> std::string_view {
            using raw_type = std::remove_pointer_t<decltype(active_ptr)>;
            return corecad::meta::type_name<raw_type>();
        }, ptr);
    }

    const attribute_descriptor& find_attribute_descriptor(const model_variant_ptr &ptr, std::string_view attr)
    {
        const auto& all_ad = get_all_attribute_data();
        const auto tid = get_type_index(ptr);

        const auto list_it = all_ad.find(tid);
        if (list_it == all_ad.end())
        {
            throw std::runtime_error(std::format(
                "Named attributes are not supported for the requested type: '{}'"
                , get_type_name(ptr)
            ));
        }

        const auto& list = list_it->second;

        const auto it = std::ranges::find_if(list, [&](const auto& ad) { return ad.name == attr; });
        if (it != list.end())
        {
            return *it;
        }

        throw std::runtime_error(std::format(
            "Requested attribute '{}' does not exist for type '{}'"
            , attr
            , get_type_name(ptr)
        ));
    }
}

attribute_service::attribute_service(const floor &f)
    : _floor { f }
{
}

const std::vector<attr_info> &attribute_service::list_attributes(model_variant_ptr ptr) const
{
    static auto all_attr_info = build_all_attr_info();
    static std::vector<attr_info> empty_result;

    const auto tid = get_type_index(ptr);

    auto it = all_attr_info.find(tid);
    if (it != all_attr_info.end())
    {
        return it->second;
    }

    return empty_result;
}

attr_variant_val attribute_service::get_attribute(model_variant_ptr ptr, std::string_view attr) const
{
    const auto& ad = find_attribute_descriptor(ptr, attr);
    return ad.reader(ptr);
}

std::optional<std::string> attribute_service::set_attribute(model_variant_ptr ptr, std::string_view attr, attr_variant_val val)
{
    auto& ad = find_attribute_descriptor(ptr, attr);
    return ad.writer(ptr, std::move(val), _floor);
}
