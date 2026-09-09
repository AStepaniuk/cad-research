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
    
    struct attribute
    {
        std::string name;
    };

    struct assignment
    {
        attribute attr;
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
