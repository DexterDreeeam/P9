#include "../../Interface.hpp"

namespace P9
{

namespace Platform
{

static Storage::index_table::Set_Ty search_qualified_document(
    ref<Storage::index_table> table, Interpreter::query_criteria criteria)
{
    AUTO_TRACE;

    Storage::index_table::Set_Ty rst;
    auto& table_map = table->get_control();
    escape_function ef =
        [=]() mutable
        {
            table->put_control();
        };

    boole insert_item = boole::False;
    auto factor = criteria.factor->clone();
    if (table_map.count(factor) == 0)
    {
        table_map.insert(factor, Storage::index_table::Set_Ty());
        insert_item = boole::True;
    }
    auto itr = table_map.find(factor);
    switch (criteria.op)
    {
    case Interpreter::query_operator::EQUAL:
        rst = itr->second;
        break;
    case Interpreter::query_operator::GREATER_EQUAL:
        rst = itr->second;
    case Interpreter::query_operator::GREATER:
        {
            while (++itr != table_map.end())
            {
                for (auto& s : itr->second)
                {
                    rst.insert(s);
                }
            }
        }
        break;
    case Interpreter::query_operator::LESS_EQUAL:
        rst = itr->second;
    case Interpreter::query_operator::LESS:
        {
            auto left_itr = table_map.begin();
            while (left_itr != itr)
            {
                for (auto& s : left_itr->second)
                {
                    rst.insert(s);
                }
                ++left_itr;
            }
        }
        break;
    default:
        assert_info(0, "unexpected compare operator.");
        break;
    }
    if (insert_item)
    {
        table_map.erase(factor);
    }
    return rst;
}

void platform::load(const string& location)
{
    AUTO_TRACE;

    _storage = new Storage::storage(location);
    _interpreter = new Interpreter::standard_interpreter();
}

ref<Interpreter::query_operation> platform::parse_operation_message(const string& str)
{
    AUTO_TRACE;

    if (!_interpreter)
    {
        assert_info(0, "P9DB Platform doesn't load interpreter.");
        return ref<Interpreter::query_operation>();
    }

    return _interpreter->translate(str);
}

string platform::handle_operation(ref<Interpreter::query_operation> op)
{
    AUTO_TRACE;

    switch (op->type())
    {
    case Interpreter::query_operation_type::UPSERT:
        return handle_upsert(op.ref_of<Interpreter::query_operation_upsert>());
    case Interpreter::query_operation_type::RETRIEVE:
        return handle_retrieve(op.ref_of<Interpreter::query_operation_retrieve>());
    case Interpreter::query_operation_type::HARD_DELETE:
        return handle_hard_delete(op.ref_of<Interpreter::query_operation_hard_delete>());
    case Interpreter::query_operation_type::SEARCH:
        return handle_search(op.ref_of<Interpreter::query_operation_search>());
    default:
        err("Not expected operation type, %d.", op->type());
        assert_info(0, "Not expected operation type.");
        break;
    }
    return string("error");
}

string platform::handle_upsert(ref<Interpreter::query_operation_upsert> op)
{
    AUTO_TRACE;
    auto partition = _storage->insert_partition(op->partition);
    auto r_document = ref<Storage::document_identifier>::new_instance(
        op->document_id, partition->location(), op->content->value());

    if (partition->upsert(r_document, op->document_etag, op->content))
    {
        return "ok";
    }
    else
    {
        return "etag-conflict";
    }
}

string platform::handle_retrieve(ref<Interpreter::query_operation_retrieve> op)
{
    AUTO_TRACE;
    auto partition = _storage->get_partition(op->partition);
    if (!partition)
    {
        return string("partition not found");
    }
    auto document_table = partition->get_document_table();
    auto r_doc = document_table->get_document(op->document_id);
    if (!r_doc)
    {
        return string("document id not found");
    }
    return transform_result({ r_doc });
}

string platform::handle_hard_delete(ref<Interpreter::query_operation_hard_delete> op)
{
    AUTO_TRACE;
    auto partition = _storage->get_partition(op->partition);
    partition->hard_delete(op->document_id);
    return "ok";
}

string platform::handle_search(ref<Interpreter::query_operation_search> op)
{
    AUTO_TRACE;
    auto partition = _storage->get_partition(op->partition);
    if (!partition)
    {
        return string("partition not found");
    }
    if (op->syntax.criterion.size() == 0)
    {
        return string("criterion should not be empty");
    }

    vector<ref<Storage::document_identifier>> vrdi;
    Storage::index_table::Set_Ty qualified_documents;
    boole is_first_search = boole::True;
    auto docs = partition->get_document_table();

    for (auto& criteria : op->syntax.criterion)
    {
        auto table = partition->get_index_table(criteria.path);
        if (table.invalid())
        {
            err("criteria is error. path: %s.", criteria.path.data());
            goto L_invalid_criteria;
        }
        auto search_documents = search_qualified_document(table, criteria);
        if (is_first_search)
        {
            qualified_documents = search_documents;
            is_first_search = boole::False;
        }
        else
        {
            Storage::index_table::Set_Ty temp;
            auto &small_set = search_documents.size() < qualified_documents.size() ? search_documents : qualified_documents;
            auto &big_set = search_documents.size() < qualified_documents.size() ? qualified_documents : search_documents;

            for (const auto& doc : small_set)
            {
                if (big_set.count(doc))
                {
                    temp.insert(doc);
                }
            }
            qualified_documents = temp;
        }
        if (qualified_documents.size() == 0)
        {
            goto L_no_result;
        }
    }

    for (auto doc : qualified_documents)
    {
        vrdi.push_back(doc);
    }
    return transform_result(vrdi);

L_invalid_criteria:
    return "invalid criteria";

L_no_result:
    return "no result found";
}

ref<json_object> transform_document_identifier(ref<Storage::document_identifier> rdi)
{
    AUTO_TRACE;

    auto jobject = ref<json_object>::new_instance();
    jobject->add_item("document_id", json_string::new_instance(rdi->_document_id));
    jobject->add_item("document_etag", json_string::new_instance(rdi->_etag));
    jobject->add_item("document_content", json_string::new_instance(rdi->read()));
    return jobject;
}

string platform::transform_result(const vector<ref<Storage::document_identifier>>& vrdi)
{
    AUTO_TRACE;

    auto jarray = ref<json_array>::new_instance();

    for (auto& rdi : vrdi)
    {
        jarray->add_item(transform_document_identifier(rdi));
    }

    return jarray->value();
}

}
}
