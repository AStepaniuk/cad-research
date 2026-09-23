#pragma once

#include <string>
#include <vector>
#include <variant>

#include "units.h"

namespace gui::cmd_parser
{
    struct value
    {
        corecad::model::length_mm_t data;
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
