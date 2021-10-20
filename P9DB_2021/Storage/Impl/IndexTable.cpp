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

    if (document_map.count(document_id))
    {
        rst = document_map[document_id];
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
    boole rst = boole::False;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    if (document_map.count(document_id))
    {
        if (document_map.erase(document_id))
        {
            rst = boole::True;
        }
    }
    return rst;
}

set<string> index_table::get_documents(ref<json_base> key)
{
    set<string> rst;
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    if (document_map.count(key))
    {
        rst = document_map[key];
    }
    return rst;
}

boole index_table::insert_document(ref<json_base> key, const string& document_id)
{
    boole ret = boole::False;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto& document_id_set = document_map[key];
    if (document_id_set.count(document_id) == 0)
    {
        document_id_set.insert(document_id);
        ret = boole::True;
    }
    return ret;
}

boole index_table::remove_document(ref<json_base> key, const string& document_id)
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
        auto& document_id_set = itr->second;
        auto itr_document = document_id_set.find(document_id);
        if (itr_document != document_id_set.end())
        {
            document_id_set.erase(itr_document);
            ret = boole::True;
            if (document_id_set.size() == 0)
            {
                document_map.erase(itr);
            }
        }
    }
    return ret;
}

}
}
