#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <format>


namespace gui::cmd_parser
{
    class parser_error : public std::runtime_error
    {
    public:
        parser_error(std::string_view failed_text, std::size_t position)
            : std::runtime_error(std::format("Syntax error . At position {}; unexpected token: '{}'", position, failed_text))
            , _failed_text(failed_text)
            , _position(position)
        {}

        [[nodiscard]] std::string_view failed_text() const noexcept
        { 
            return _failed_text; 
        }

        [[nodiscard]] std::size_t position() const noexcept
        { 
            return _position; 
        }

    private:
        std::string _failed_text;
        std::size_t _position;
    };
}
