#include "../Interface.h"

namespace P9
{

namespace Interpreter
{

boole query_order::load(JsonNs::json_base* json)
{
    auto j = json_cursor(json);
    if (j.invalid() || j->type() != json_type::OBJECT)
    {
        return boole::False;
    }

    auto j_path = j["path"];
    if (j_path.invalid() || j_path->type() != json_type::STRING)
    {
        return boole::False;
    }
    path = j_path->value();

    auto j_ascend = j["ascend"];
    if (j_ascend.invalid() || j_ascend->type() != json_type::BOOLE)
    {
        return boole::False;
    }
    ascend = j_ascend.as_boole();

    return boole::True;
}

boole query_criteria::load(JsonNs::json_base* json)
{
    auto j = json_cursor(json);
    if (j.invalid() || j->type() != json_type::OBJECT)
    {
        return boole::False;
    }

    auto j_path = j["path"];
    if (j_path.invalid() || j_path->type() != json_type::STRING)
    {
        return boole::False;
    }
    path = j_path->value();

    auto j_op = j["operator"];
    if (j_op.invalid() || j_op->type() != json_type::STRING)
    {
        return boole::False;
    }
    string str_op = j_op->value();
    if (str_op == "=")
    {
        op = query_operator::EQUAL;
    }
    else if (str_op == ">")
    {
        op = query_operator::GREATER;
    }
    else if (str_op == ">=")
    {
        op = query_operator::GREATER_EQUAL;
    }
    else if (str_op == "<")
    {
        op = query_operator::LESS;
    }
    else if (str_op == "<=")
    {
        op = query_operator::LESS_EQUAL;
    }
    else
    {
        return boole::False;
    }

    auto j_factor = j["factor"];
    if (j_factor.invalid())
    {
        return boole::False;
    }

    factor = j_factor->clone();

    return boole::True;
}

boole query_syntax::load(JsonNs::json_base* json)
{
    AUTO_TRACE;

    auto j = json_cursor(json);
    if (j.invalid() || j->type() != json_type::OBJECT)
    {
        return boole::False;
    }

    auto j_order = j["order"];
    if (j_order.invalid() || j_order->type() != json_type::OBJECT)
    {
        return boole::False;
    }
    if (order.load(j_order.json()) == boole::False)
    {
        return boole::False;
    }

    auto j_criterion = j["criterion"];
    if (j_criterion.invalid() || j_criterion->type() != json_type::ARRAY)
    {
        return boole::False;
    }
    for (s64 i = 0; i < j_criterion->size(); ++i)
    {
        query_criteria criteria;
        if (criteria.load(j_criterion[i].json()) == boole::False)
        {
            return boole::False;
        }
    }

    return boole::True;
}

}
}
