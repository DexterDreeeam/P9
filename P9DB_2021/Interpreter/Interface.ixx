module;
export module P9DB.Interpreter;

import P9DB.QueryOperation;
import P9DB.Interpreter.Base;
import P9DB.Interpreter.Standard;
import P9DB.Interpreter.Straight;

namespace P9
{

namespace Interpreter
{

export base_interpreter* build_interpreter(interpreter_type type)
{
    if (type == interpreter_type::STANDARD)
    {
        return new standard_interpreter();
    }
    return nullptr;
}

}
}
