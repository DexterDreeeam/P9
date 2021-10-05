module;
#include "../../P9SDK/Interface/Interface.h"
export module P9DB.Interpreter.Base;

export import P9DB.QueryOperation;

namespace P9
{

export namespace Interpreter
{

enum class interpreter_type : s64
{
    STANDARD,
};

class base_interpreter
{
public:
    base_interpreter() = default;

    virtual ~base_interpreter() = default;

    virtual QueryOperation::query_operation* translate(const string& msg) = 0;
};

}
}
