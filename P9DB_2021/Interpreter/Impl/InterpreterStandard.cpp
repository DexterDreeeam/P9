#include "../Interface.h"

namespace P9
{

namespace Interpreter
{

standard_interpreter::standard_interpreter() :
    base_interpreter()
{
    AUTO_TRACE;
}

standard_interpreter::~standard_interpreter()
{
    AUTO_TRACE;
}

query_operation* standard_interpreter::translate(const string& msg)
{
    AUTO_TRACE;

    auto* json = JsonDeserialize(msg);
    json_cursor j_base(json);
    json_cursor j_idx;
    query_operation* rst = nullptr;
    string str_operation;
    s64 s64_submit_time;
    string str_user_token;
    string str_partition;

    if (j_base.invalid())
    {
        goto L_error;
    }

    j_idx = j_base["operation"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        goto L_error;
    }
    str_operation = j_idx->value();

    j_idx = j_base["submit_time"];
    if (j_idx.invalid() || j_idx->type() != json_type::INT)
    {
        goto L_error;
    }
    s64_submit_time = j_idx.as_s64();

    j_idx = j_base["user_token"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        goto L_error;
    }
    str_user_token = j_idx->value();

    j_idx = j_base["partition"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        goto L_error;
    }
    str_partition = j_idx->value();

    if (str_operation == "upsert")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            goto L_error;
        }
        string str_document_id = j_idx->value();

        j_idx = j_base["document_etag"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            goto L_error;
        }
        string str_document_etag = j_idx->value();

        auto content = j_base["content"];
        if (j_idx.invalid() || j_idx->type() != json_type::OBJECT)
        {
            goto L_error;
        }

        auto* upsert = new query_operation_upsert();
        upsert->document_id = str_document_id;
        upsert->document_etag = str_document_etag;
        upsert->content = content->clone();
        rst = upsert;
    }
    else if (str_operation == "retrieve")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            goto L_error;
        }
        string str_document_id = j_idx->value();

        auto* retrieve = new query_operation_retrieve();
        retrieve->document_id = str_document_id;
        rst = retrieve;
    }
    else if (str_operation == "hard_delete")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            goto L_error;
        }
        string str_document_id = j_idx->value();

        auto* hard_delete = new query_operation_hard_delete();
        hard_delete->document_id = str_document_id;
        rst = hard_delete;
    }
    else if (str_operation == "search_single" || str_operation == "search_range")
    {
        j_idx = j_base["syntax"];
        if (j_idx.invalid() || j_idx->type() != json_type::OBJECT)
        {
            goto L_error;
        }

        query_operation_search* search = nullptr;
        if (str_operation == "search_single")
        {
            search = new query_operation_search_single();
        }
        else if (str_operation == "search_range")
        {
            search = new query_operation_search_range();
        }
        else
        {
            goto L_error;
        }
        if (search->load(j_idx.json()) == boole::False)
        {
            err("%s: search syntax error '%s'.", __FUNCTION__, j_idx->value());
            goto L_error;
        }
        rst = search;
    }
    else
    {
        err("%s: unexpected operation '%s'.", __FUNCTION__, str_operation.data());
        goto L_error;
    }

    rst->submit_time = (u64)s64_submit_time;
    rst->user_token = str_user_token;
    rst->partition = str_partition;
    rst->time = date_timestamp_utc();

    goto L_finish;

L_error:
    log("%s: process error.", __FUNCTION__);
    if (rst != nullptr)
    {
        delete rst;
    }

L_finish:
    if (json)
    {
        delete json;
    }
    return rst;
}

}
}
