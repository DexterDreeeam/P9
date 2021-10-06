#pragma once

namespace P9
{

namespace Interpreter
{

enum class query_operation_type : s64
{
    UPSERT,

    RETRIEVE,

    SEARCH_SINGLE,
    SEARCH_RANGE,

    SOFT_DELETE,
    HARD_DELETE,
};

class query_operation
{
public:
    query_operation() = default;

    virtual ~query_operation() = default;

    virtual query_operation_type type() = 0;

public:
    u64     submit_time;
    u64     time;
    string  user_token;
    string  partition;
};

class query_operation_upsert : public query_operation
{
public:
    query_operation_upsert() :
        query_operation()
    {}

    virtual ~query_operation_upsert() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::UPSERT;
    }

public:
    string  document_id;
    string  document_etag;
    JsonNs::json_base* content;
};

class query_operation_retrieve : public query_operation
{
public:
    query_operation_retrieve() :
        query_operation()
    {}

    virtual ~query_operation_retrieve() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::RETRIEVE;
    }

public:
    string  document_id;
};

class query_operation_hard_delete : public query_operation
{
public:
    query_operation_hard_delete() :
        query_operation()
    {}

    virtual ~query_operation_hard_delete() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::HARD_DELETE;
    }

public:
    string  document_id;
};

class query_operation_search : public query_operation
{
public:
    query_operation_search() :
        query_operation(),
        syntax()
    {}

    virtual ~query_operation_search() override
    {}

    boole load(JsonNs::json_base* json_syntax)
    {
        return syntax.load(json_syntax);
    }

public:
    query_syntax syntax;
};

class query_operation_search_single : public query_operation_search
{
public:
    query_operation_search_single() :
        query_operation_search()
    {}

    virtual ~query_operation_search_single() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::SEARCH_SINGLE;
    }
};

class query_operation_search_range : public query_operation_search
{
public:
    query_operation_search_range() :
        query_operation_search()
    {}

    virtual ~query_operation_search_range() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::SEARCH_RANGE;
    }
};

}
}
