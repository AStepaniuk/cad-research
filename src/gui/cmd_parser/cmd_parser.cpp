#include "cmd_parser.h"

#include <utility>
#include <charconv>
#include <stdexcept>
#include <format>

#include "parser_error.h"

using namespace gui::cmd_parser;

namespace
{
    template <typename TNode>
    using parse_result = std::pair<
        std::optional<TNode>,       // parsed node (if matched)
        size_t                      // number of consumed characters
    >;
    
    std::string_view advance(std::string_view source, size_t bytes_num)
    {
        if (bytes_num > source.size())
        {
            throw std::out_of_range(std::format( "Parse internal error. Consumed {} bytes, but available {} bytes", bytes_num, source.size()));
        } 

        return source.substr(bytes_num);
    }

    bool is_first_identifier_char(char c)
    {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    bool is_next_identifier_char(char c)
    {
        return is_first_identifier_char(c) || std::isdigit(c);
    }

    bool is_number_char(char c)
    {
        return std::isdigit(static_cast<unsigned char>(c))
            || c == '.'
            || c == '-'
            || c == '+';
    }

    size_t skip_whitespaces(std::string_view source)
    {
        size_t cursor = 0;

        while (cursor < source.size() && std::isspace(static_cast<unsigned char>(source[cursor])))
        {
            cursor++;
        }

        return cursor;
    }

    parse_result<std::string> parse_identifier(std::string_view source)
    {
        if (source.empty() || !is_first_identifier_char(static_cast<unsigned char>(source[0])))
        {
            return { std::nullopt, 0 };
        }

        size_t cursor = 1;
        while (cursor < source.size() && is_next_identifier_char(static_cast<unsigned char>(source[cursor])))
        {
            cursor++;
        }

        return { std::string(source.data(), cursor), cursor };
    }

    parse_result<double> parse_double(std::string_view source)
    {
        size_t cursor = 0;
        while (cursor < source.size() && is_number_char(static_cast<unsigned char>(source[cursor])))
        {
            cursor++;
        }

        if (cursor == 0)
        {
            return { std::nullopt, 0 };
        }

        double numeric_data = 0.0;
        auto [ptr, ec] = std::from_chars(source.data(), source.data() + cursor, numeric_data);
        
        if (ec != std::errc{})
        {
            return { std::nullopt, 0 };
        }

        return { numeric_data, cursor };
    }

    parse_result<attribute> parse_attribute(std::string_view source)
    {
        auto [name, name_size] = parse_identifier(source);
        if (name)
        {
            return { attribute { .name = std::move(name.value()) }, name_size };
        }
        else
        {
            return { std::nullopt, 0 };
        }
    }

    parse_result<value> parse_value(std::string_view source)
    {
        auto [num, num_size] = parse_double(source);
        if (num)
        {
            return { value { .data = num.value() }, num_size };
        }
        else
        {
            return { std::nullopt, 0 };
        }
    }
    
    parse_result<assignment> parse_assignment(std::string_view source)
    {
        size_t cursor = 0;

        auto [attr, attr_size] = parse_attribute(source);
        if (!attr)
        {
            return { std::nullopt, 0 };
        }
        cursor += attr_size;

        cursor += skip_whitespaces(advance(source, cursor));

        if (cursor >= source.size() || source[cursor] != '=')
        {
            return { std::nullopt, 0 };
        }
        cursor += 1;

        cursor += skip_whitespaces(advance(source, cursor));

        auto [val, val_size] = parse_value(advance(source, cursor));
        if (!val)
        {
            return { std::nullopt, 0 };
        }
        cursor += val_size;

        return {
            assignment
            {
                .attr = std::move(attr.value()),
                .val = std::move(val.value())
            },
            cursor
        };
    }

    parse_result<instructions> parse_instructions(std::string_view source)
    {
        instructions result;

        size_t cursor = 0;
        while (cursor < source.size())
        {
            cursor += skip_whitespaces(advance(source, cursor));

            if (cursor == source.size())
            {
                break;
            }

            const auto instr_start = cursor;
            auto [assignment, instr_size] = parse_assignment(advance(source, cursor));
            if (!assignment)
            {
                return { std::nullopt, 0 };
            }
            cursor += instr_size;

            result.list.push_back(instruction_info {
                .instr = std::move(assignment.value()),
                .src_text = std::string { source.substr(instr_start, instr_size) }
            });
        }

        return { result, cursor };
    }
}

instructions parser::parse(std::string_view source)
{
    auto [instrs, instrs_size] = parse_instructions(source);

    if (!instrs || instrs_size < source.size())
    {
        auto error_pos = std::min(instrs_size, source.size());
        std::string failed_text = std::string { source.substr(error_pos) };
        throw parser_error(failed_text, error_pos);
    }

    return instrs.value();
}
