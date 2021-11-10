#include "../../Interface.hpp"

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

    auto* json = json_deserialize(msg);
    assert_info(json, "Translate message is not a correct Json format.");

    json_cursor j_base(json);
    json_cursor j_idx;
    ref<query_operation> rst;
    string str_operation;
    number s64_submit_time;
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
        assert_info(0, "Query operation parsing error.");
        goto L_error;
    }
    str_operation = j_idx->value();
    str_operation = remove_quotation(str_operation);
    log("operation is %s", str_operation.data());

    j_idx = j_base["submit_time"];
    if (j_idx.invalid() || j_idx->type() != json_type::NUMBER)
    {
        err("Query submit_time parsing error.");
        assert_info(0, "Query submit_time parsing error.");
        goto L_error;
    }
    s64_submit_time = j_idx.as_number();

    j_idx = j_base["user_token"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        err("Query user_token parsing error.");
        assert_info(0, "Query user_token parsing error.");
        goto L_error;
    }
    str_user_token = j_idx->value();
    str_user_token = remove_quotation(str_user_token);

    j_idx = j_base["partition"];
    if (j_idx.invalid() || j_idx->type() != json_type::STRING)
    {
        err("Query partition parsing error.");
        assert_info(0, "Query partition parsing error.");
        goto L_error;
    }
    str_partition = j_idx->value();
    str_partition = remove_quotation(str_partition);

    if (str_operation == "upsert")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("Query document_id parsing error.");
            assert_info(0, "Query document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        j_idx = j_base["document_etag"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("Query document_etag parsing error.");
            assert_info(0, "Query document_etag parsing error.");
            goto L_error;
        }
        string str_document_etag = j_idx->value();
        str_document_etag = remove_quotation(str_document_etag);

        auto content = j_base["content"];
        if (content.invalid() || content->type() != json_type::OBJECT)
        {
            err("Query content parsing error.");
            assert_info(0, "Query content parsing error.");
            goto L_error;
        }

        auto upsert = ref<query_operation_upsert>::new_instance();
        upsert->document_id = str_document_id;
        upsert->document_etag = str_document_etag;
        upsert->content = content->clone();
        rst = upsert.ref_of<query_operation>();
    }
    else if (str_operation == "retrieve")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("Query document_id parsing error.");
            assert_info(0, "Query document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        auto retrieve = ref<query_operation_retrieve>::new_instance();
        retrieve->document_id = str_document_id;
        rst = retrieve.ref_of<query_operation>();
    }
    else if (str_operation == "hard_delete")
    {
        j_idx = j_base["document_id"];
        if (j_idx.invalid() || j_idx->type() != json_type::STRING)
        {
            err("Query document_id parsing error.");
            assert_info(0, "Query document_id parsing error.");
            goto L_error;
        }
        string str_document_id = j_idx->value();
        str_document_id = remove_quotation(str_document_id);

        auto hard_delete = ref<query_operation_hard_delete>::new_instance();
        hard_delete->document_id = str_document_id;
        rst = hard_delete.ref_of<query_operation>();
    }
    else if (str_operation == "search")
    {
        j_idx = j_base["syntax"];
        if (j_idx.invalid() || j_idx->type() != json_type::OBJECT)
        {
            err("Query syntax parsing error.");
            assert_info(0, "Query syntax parsing error.");
            goto L_error;
        }

        ref<query_operation_search> search = ref<query_operation_search>::new_instance();
        if (search->load(j_idx.json()) == boole::False)
        {
            err("%s: search syntax error '%s'.", __FUNCTION__, j_idx->value().data());
            assert_info(0, "Query search syntax parsing error.");
            goto L_error;
        }
        rst = search.ref_of<query_operation>();
    }
    else
    {
        err("Unexpected error happens when translate query '%s'.", __FUNCTION__, str_operation.data());
        assert_info(0, "Unexpected error happens when translate query.");
        goto L_error;
    }

    rst->submit_time = s64_submit_time;
    rst->user_token = str_user_token;
    rst->partition = str_partition;
    rst->time = date::timestamp();

    goto L_finish;

L_error:
    err("%s: process error.", __FUNCTION__);
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
