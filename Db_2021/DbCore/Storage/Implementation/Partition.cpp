#include "../../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<index_table> partition::get_index_table(const string& index_table_name)
{
    AUTO_TRACE;

    ref<index_table> rst;
    _op_lock.wait_read();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.read_release();
        };

    if (_index_table_map.count(index_table_name))
    {
        rst = _index_table_map[index_table_name];
    }
    return rst;
}

ref<index_table> partition::insert_index_table(const string& index_table_name)
{
    AUTO_TRACE;

    auto new_table = ref<index_table>::new_instance();
    ref<index_table> rst;
    _op_lock.wait_write();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.write_release();
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
    AUTO_TRACE;

    _op_lock.wait_write();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.write_release();
        };

    _index_table_map.erase(index_table_name);
}

boole partition::upsert(ref<document_identifier> r_doc, const string& op_etag, ref<json_base> content)
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

void partition::build_document_index(ref<document_identifier> r_doc, ref<json_base> content)
{
    AUTO_TRACE;

    _op_lock.wait_write();
    escape_function ef =
        [=]() mutable
    {
        _op_lock.write_release();
    };

    content->iterate(
        [&](ref<json_base> json)
        {
            string index_string = json->my_path_index_string();
            if (index_string.contains(".*"))
            {
                return;
            }
            auto table = insert_index_table(index_string);
            auto json_value = json->clone();
            table->insert_document(json_value, r_doc);
        },
        // leaves only
        boole::True);
}

void partition::clear_document_index(ref<document_identifier> r_doc)
{
    AUTO_TRACE;

    string content_str = r_doc->read();
    ref<json_base> content = json_deserialize(content_str);
    _op_lock.wait_read();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.read_release();
        };

    content->iterate(
        [&](ref<json_base> json)
        {
            string index_string = json->my_path_index_string();
            if (index_string.contains(".*"))
            {
                return;
            }
            auto table = _index_table_map[index_string];
            table->remove_document(json->clone(), r_doc);
        },
        // leaves only
        boole::True);
}

boole partition::hard_delete(const string& document_id)
{
    AUTO_TRACE;

    auto r_doc = _document_map->get_document(document_id);

    _op_lock.wait_read();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.read_release();
        };

    clear_document_index(r_doc);
    _document_map->remove_document(document_id);

    return boole::True;
}

}
}
