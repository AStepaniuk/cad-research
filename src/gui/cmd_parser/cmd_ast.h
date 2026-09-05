#pragma once

#include <string>
#include <vector>
#include <variant>

namespace gui::cmd_parser
{
    struct value
    {
        double data;
    };
    
    struct parameter
    {
        std::string name;
    };

    struct assignment
    {
        parameter param;
        value val;
    };

    using instruction = std::variant<assignment>;

    struct instruction_info
    {
        instruction instr;
        std::string src_text;
    };

    struct instructions
    {
        std::vector<instruction_info> list;
    };
}
