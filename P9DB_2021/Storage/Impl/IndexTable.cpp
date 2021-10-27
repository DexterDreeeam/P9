#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<document_identifier> index_document_table::get_document(const string& document_id)
{
    ref<document_identifier> rst;
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    auto itr = document_map.find(document_id);
    if (itr != document_map.end())
    {
        rst = itr->second;
    }
    return rst;
}

boole index_document_table::insert_document(
    const string& document_id, ref<document_identifier> r_doc)
{
    boole rst;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    if (document_map.count(document_id) == 0)
    {
        document_map[document_id] = r_doc;
        rst = boole::True;
    }
    else
    {
        rst = boole::False;
    }
    return rst;
}

boole index_document_table::remove_document(const string& document_id)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto itr = document_map.find(document_id);
    if (itr != document_map.end())
    {
        document_map.erase(itr);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole index_document_table::upsert_process(const string& document_id, ref<document_identifier> r_doc)
{
    boole test_rst;

    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
    {
        rw_lock_put_write(_op_lock);
    };

    auto itr = document_map.find(document_id);
    if (itr == document_map.end())
    {
        document_map[document_id] = r_doc;
        return boole::True;
    }

    auto& itr_doc = itr->second;
    if (itr_doc->is_status(document_identifier_status::CREATING) || itr_doc->is_status(document_identifier_status::UPDATING))
    {
        return boole::False;
    }

    if (itr_doc->is_status(document_identifier_status::DELETING))
    {

    }
    else if (itr_doc->transform(document_identifier_status::NORMAL, document_identifier_status::UPDATING))
    {
        if (itr_doc->_etag != r_doc->_etag)
        {
            test_rst = itr_doc->transform(document_identifier_status::UPDATING, document_identifier_status::NORMAL);
            assert(test_rst);
            return boole::False;
        }
        r_doc->transform(document_identifier_status::CREATING, document_identifier_status::IMMIGRATING);

    }
    else
    {
        return boole::False;
    }
}

index_table::Set_Ty index_table::get_documents(ref<json_base> key)
{
    Set_Ty rst;

    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    auto itr = document_map.find(key);
    if (itr != document_map.end())
    {
        rst = itr->second;
    }
    return rst;
}

boole index_table::insert_document(ref<json_base> key, ref<document_identifier> identifier)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto& document_set = document_map[key];
    if (document_set.size() == 0 || document_set.find(identifier) == document_set.end())
    {
        document_set.insert(identifier);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole index_table::remove_document(ref<json_base> key, ref<document_identifier> identifier)
{
    boole ret = boole::False;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto itr = document_map.find(key);
    if (itr != document_map.end())
    {
        auto& document_set = itr->second;
        if (document_set.erase(identifier))
        {
            return boole::True;
        }
    }
    return boole::False;
}

}
}
