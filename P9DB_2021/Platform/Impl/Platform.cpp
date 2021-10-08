#include "../Interface.h"

namespace P9
{

namespace Platform
{

void platform::load(const string& location)
{
    _storage = new Storage::storage(location);
    _interpreter = new Interpreter::standard_interpreter();
}

ref<Interpreter::query_operation> platform::parse_operation_message(const string& str)
{
    if (!_interpreter)
    {
        return ref<Interpreter::query_operation>();
    }

    return _interpreter->translate(str);
}

string platform::handle_operation(ref<Interpreter::query_operation> op)
{
    switch (op->type())
    {
    case Interpreter::query_operation_type::UPSERT:
        return handle_upsert(op.cast<Interpreter::query_operation_upsert>());
    case Interpreter::query_operation_type::RETRIEVE:
        return handle_retrieve(op.cast<Interpreter::query_operation_retrieve>());
    case Interpreter::query_operation_type::HARD_DELETE:
        return handle_hard_delete(op.cast<Interpreter::query_operation_hard_delete>());
    case Interpreter::query_operation_type::SEARCH_SINGLE:
        return handle_search_single(op.cast<Interpreter::query_operation_search_single>());
    case Interpreter::query_operation_type::SEARCH_RANGE:
        return handle_search_range(op.cast<Interpreter::query_operation_search_range>());
    default:
        err("Not expected operation type, %d.", op->type());
        break;
    }
    return string("error");
}

string platform::handle_upsert(ref<Interpreter::query_operation_upsert> op)
{
    AUTO_TRACE;
    auto partition = _storage->insert_partition(op->partition);
    auto index_table = partition->insert_index_table("document_id");
    auto doc = index_table->insert_document(op->document_id);
    doc->overwrite(op->content->value());

    op->content->iterate(
        [&](JsonNs::json_base* json)
        {
            string index_string = json->my_path_index_string();
            auto table = partition->insert_index_table(index_string);
            table->insert_index(doc);
        },
        // leaves only
        boole::True);

    return "ok";
}

string platform::handle_retrieve(ref<Interpreter::query_operation_retrieve> op)
{
    AUTO_TRACE;
    auto partition = _storage->get_partition(op->partition);
    if (!partition)
    {
        return string();
    }
    auto index_table = partition->get_index_table("document_id");
    if (!index_table)
    {
        return string();
    }
    auto doc = index_table->insert_document(op->document_id);
    if (!doc)
    {
        return string();
    }
    return doc->read();
}

string platform::handle_hard_delete(ref<Interpreter::query_operation_hard_delete> op)
{
    return "";
}

string platform::handle_search_single(ref<Interpreter::query_operation_search_single> op)
{
    return "";
}

string platform::handle_search_range(ref<Interpreter::query_operation_search_range> op)
{
    return "";
}

}
}
