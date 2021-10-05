module;
export module P9DB.Interpreter.Standard;

import P9DB.QueryOperation;
import P9DB.Interpreter.Base;

namespace P9
{

export namespace Interpreter
{

class standard_interpreter : public base_interpreter
{
public:
    standard_interpreter();

    virtual ~standard_interpreter() override;

    virtual QueryOperation::query_operation* translate(const string& msg) override;
};

}
}
