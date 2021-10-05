module;

#include "../../P9SDK/Interface/Interface.h"

export module P9DB.QueryOperation;

namespace P9
{

export namespace QueryOperation
{

enum class query_operation_type : s64
{
    UPSERT,

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

protected:
    date    submit_time;
    date    time;
    string  user_token;
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
    string  partition;
    string  document_id;
    string  document_etag;
    json_object* content;
};

class query_operation_search_single : public query_operation
{
public:
    query_operation_search_single() :
        query_operation()
    {}

    virtual ~query_operation_search_single() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::SEARCH_SINGLE;
    }

public:
    string  partition;
};

class query_operation_search_range : public query_operation
{
public:
    query_operation_search_range() :
        query_operation()
    {}

    virtual ~query_operation_search_range() override
    {}

    virtual query_operation_type type() override
    {
        return query_operation_type::SEARCH_RANGE;
    }

public:
    string  partition;
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
    string  partition;
    string  document_id;
};

}
}
