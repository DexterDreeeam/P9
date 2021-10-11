#include "../Interface.hpp"

namespace P9
{

namespace Interpreter
{

string remove_quotation(const string& str)
{
    if (str.size() >= 2 && str.front() == '\"' && str.back() == '\"')
    {
        return str.substr(1, str.size() - 2);
    }
    assert(0);
    return string();
}

}
}
