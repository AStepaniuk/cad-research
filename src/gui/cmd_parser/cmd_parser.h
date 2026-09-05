#pragma once

#include <string_view>

#include "cmd_ast.h"

namespace gui::cmd_parser
{
    class parser
    {
    public:
        static instructions parse(std::string_view source);
    };
}

