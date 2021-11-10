#pragma once

namespace P9
{

namespace Interpreter
{

enum class interpreter_type : s64
{
    STANDARD,
};

class base_interpreter : object
{
public:
    base_interpreter()
    {
        AUTO_TRACE;
    }

    virtual ~base_interpreter()
    {
        AUTO_TRACE;
    }

    virtual ref<query_operation> translate(const string& msg) = 0;
};

}
}
