#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<index_table> partition::get_index_table(const string& index_table_name)
{
    ref<index_table> rst;
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    if (_index_table_map.count(index_table_name))
    {
        rst = _index_table_map[index_table_name];
    }
    return rst;
}

ref<index_table> partition::insert_index_table(const string& index_table_name)
{
    auto new_table = ref<index_table>::new_instance();
    ref<index_table> rst;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    if (_index_table_map.count(index_table_name) == 0)
    {
        _index_table_map[index_table_name] = new_table;
        rst = new_table;
    }
    else
    {
        rst = _index_table_map[index_table_name];
    }
    return rst;
}

void partition::remove_index_table(const string& index_table_name)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    _index_table_map.erase(index_table_name);
}

boole partition::upsert(ref<document_identifier> r_doc, const string& op_etag, json_base* content)
{
    AUTO_TRACE;

    boole check_rst;
    document_identifier_transform_result transform_rst;

    auto ret_doc = _document_map->try_insert_document(r_doc);
    check_rst = r_doc->is_status(document_identifier_status::CREATING);
    assert(check_rst);

    if (r_doc == ret_doc)
    {
        // insert
        build_document_index(r_doc, content);
        transform_rst = r_doc->transform(document_identifier_status::CREATING, document_identifier_status::NORMAL);
        assert(transform_rst == document_identifier_transform_result::SUCCEED);
        return boole::True;
    }

    if (ret_doc->transform_if_etag_identical(document_identifier_status::NORMAL, document_identifier_status::UPDATING, op_etag)
            == document_identifier_transform_result::SUCCEED ||
        ret_doc->transform(document_identifier_status::DELETING, document_identifier_status::DELETING)
            == document_identifier_transform_result::SUCCEED)
    {
        build_document_index(r_doc, content);
        transform_rst = r_doc->transform(document_identifier_status::CREATING, document_identifier_status::NORMAL);
        assert(transform_rst == document_identifier_transform_result::SUCCEED);
        _document_map->insert_or_replace_document(r_doc);
        clear_document_index(ret_doc);
        return boole::True;
    }

    return boole::False;
}

void partition::build_document_index(ref<document_identifier> r_doc, json_base* content)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
    {
        rw_lock_put_write(_op_lock);
    };

    content->iterate(
        [&](json_base* json)
        {
            string index_string = json->my_path_index_string();
            auto table = insert_index_table(index_string);
            auto json_value = ref<json_base>::new_instance(json->clone());
            table->insert_document(json_value, r_doc);
        },
        // leaves only
        boole::True);
}

void partition::clear_document_index(ref<document_identifier> r_doc)
{
    string content_str = r_doc->read();
    json_base* content = json_deserialize(content_str);
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    content->iterate(
        [&](json_base* json)
        {
            string index_string = json->my_path_index_string();
            auto table = _index_table_map[index_string];
            table->remove_document(ref<json_base>::new_instance(json->clone()), r_doc);
        },
        // leaves only
        boole::True);
}

boole partition::hard_delete(const string& document_id)
{
    auto r_doc = _document_map->get_document(document_id);

    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    clear_document_index(r_doc);
    _document_map->remove_document(document_id);

    return boole::True;
}

}
}
