module P9DB.Interpreter.Standard;

namespace P9
{

namespace Interpreter
{

standard_interpreter::standard_interpreter() :
    base_interpreter()
{}

standard_interpreter::~standard_interpreter()
{}

QueryOperation::query_operation* standard_interpreter::translate(const string& msg)
{
    //JsonDeserialize(msg);
    return nullptr;
}

}
}
