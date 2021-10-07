#pragma once

namespace P9
{

namespace Interpreter
{

class standard_interpreter : public base_interpreter
{
public:
    standard_interpreter();

    virtual ~standard_interpreter() override;

    virtual ref<query_operation> translate(const string& msg) override;
};

}
}
