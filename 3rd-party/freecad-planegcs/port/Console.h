#pragma once

#include <iostream>
#include <string>
#include <cstdarg>

namespace Base
{
    class Console
    {
    public:
        // Redirect FreeCAD's internal logs to standard C++ output
        static void Message(const char* s) { std::cout << s << std::endl; }
        static void message(const char* s) { std::cout << s << std::endl; }

        static void Warning(const char* s) { std::cerr << "Warning: " << s << std::endl; }
        static void warning(const char* s) { std::cerr << "Warning: " << s << std::endl; }

        static void Error(const char* s)   { std::cerr << "Error: " << s << std::endl; }
        static void error(const char* s)   { std::cerr << "Error: " << s << std::endl; }

        static void log(const char* format, ...)
        {
            va_list args;
            va_start(args, format);
            vprintf(format, args);
            va_end(args);
        }

        static void log(const std::string& s)
        {
            std::cout << s << std::endl;
        }

        
        Console& operator()() { return *this; }
    };
}

#define BaseConsole Base::Console()
