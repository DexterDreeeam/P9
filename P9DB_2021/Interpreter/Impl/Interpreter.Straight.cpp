module P9DB.Interpreter.Straight;

namespace P9
{

namespace Interpreter
{

straight_interpreter::straight_interpreter() :
    base_interpreter()
{}

straight_interpreter::~straight_interpreter()
{}

QueryOperation::query_operation* straight_interpreter::translate(const string& msg)
{
    return nullptr;
}

}
}
