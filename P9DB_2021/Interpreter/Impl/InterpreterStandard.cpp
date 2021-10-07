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

ref<query_operation> standard_interpreter::translate(const string& msg)
{
    AUTO_TRACE;

    auto* json = JsonDeserialize(msg);
    json_cursor j_base(json);
    json_cursor j_idx;
    ref<query_operation> rst;
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
        err("operation parsing error.");
        goto L_error;
    }
    str_operation = j_idx->value();
    str_operation = remove_quotation(str_operation);
    log("operation is %s", str_operation.data());

    j_idx = j_base["submit_time"];
    if (j_idx.invalid() || j_idx->type() != json_type::INT)
    {
        err("submit_time parsing error.");
        goto L_error;
    }
    s64_submit_time = j_idx.as_s64();

    j_idx = j_base["user_token"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        err("user_token parsing error.");
        goto L_error;
    }
    str_user_token = j_idx->value();
    str_user_token = remove_quotation(str_user_token);

    j_idx = j_base["partition"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        err("partition parsing error.");
        goto L_error;
    }
    str_partition = j_idx->value();
    str_partition = remove_quotation(str_partition);

    if (str_operation == "upsert")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        j_idx = j_base["document_etag"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("document_etag parsing error.");
            goto L_error;
        }
        string str_document_etag = j_idx->value();
        str_document_etag = remove_quotation(str_document_etag);

        auto content = j_base["content"];
        if (content.invalid() || content->type() != json_type::OBJECT)
        {
            err("content parsing error.");
            goto L_error;
        }

        auto upsert = ref<query_operation_upsert>::new_instance();
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
            err("document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        auto retrieve = ref<query_operation_retrieve>::new_instance();
        retrieve->document_id = str_document_id;
        rst = retrieve;
    }
    else if (str_operation == "hard_delete")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        auto hard_delete = ref<query_operation_hard_delete>::new_instance();
        hard_delete->document_id = str_document_id;
        rst = hard_delete;
    }
    else if (str_operation == "search_single" || str_operation == "search_range")
    {
        j_idx = j_base["syntax"];
        if (j_idx.invalid() || j_idx->type() != json_type::OBJECT)
        {
            err("syntax parsing error.");
            goto L_error;
        }

        ref<query_operation_search> search;
        if (str_operation == "search_single")
        {
            search = ref<query_operation_search_single>::new_instance();
        }
        else if (str_operation == "search_range")
        {
            search = ref<query_operation_search_range>::new_instance();
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
        assert(0);
        goto L_error;
    }

    rst->submit_time = (u64)s64_submit_time;
    rst->user_token = str_user_token;
    rst->partition = str_partition;
    rst->time = date_timestamp_utc();

    goto L_finish;

L_error:
    log("%s: process error.", __FUNCTION__);
    rst.clear();

L_finish:
    if (json)
    {
        delete json;
    }
    return rst;
}

}
}
