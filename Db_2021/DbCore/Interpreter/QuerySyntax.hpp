#pragma once

namespace P9
{

namespace Interpreter
{

class query_order : object
{
public:
    query_order()
    {}

    ~query_order()
    {}

    boole load(json_base * json);

public:
    string path;
    boole ascend;
};

enum class query_operator : s64
{
    INVALID = -1,
    BASE = 0,

    EQUAL = BASE,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    COUNT
};

class query_criteria : object
{
public:
    query_criteria()
    {}

    query_criteria(const query_criteria& rhs) :
        path(rhs.path),
        op(rhs.op),
        factor(rhs.factor->clone())
    {
    }

    ~query_criteria()
    {
        if (factor)
        {
            delete factor;
        }
    }

    boole load(json_base* json);

public:
    string path;
    query_operator op;
    json_base* factor;
};

class query_syntax : object
{
public:
    query_syntax()
    {}

    ~query_syntax()
    {}

    boole load(json_base * json);

public:
    query_order order;
    vector<query_criteria> criterion;
};

}
}
