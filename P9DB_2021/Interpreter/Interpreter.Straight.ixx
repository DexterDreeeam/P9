module;
#include "../../P9SDK/Interface/Interface.h"
export module P9DB.Interpreter.Straight;

import P9DB.QueryOperation;
import P9DB.Interpreter.Base;

namespace P9
{

export namespace Interpreter
{

class straight_interpreter : public base_interpreter
{
public:
    straight_interpreter();

    virtual ~straight_interpreter() override;

    virtual QueryOperation::query_operation* translate(const string& msg) override;
};

}
}
