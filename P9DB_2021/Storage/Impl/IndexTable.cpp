#include "../Interface.h"

namespace P9
{

namespace Storage
{

ref<document_identifier> index_document_id::get_document(const string& document_id)
{
    ref<document_identifier> rst;
    rw_lock_wait_read(_op_lock);
    if (document_map.count(document_id))
    {
        rst = document_map[document_id];
    }
    rw_lock_put_read(_op_lock);
    return rst;
}

boole index_document_id::insert_document(
    const string& document_id, ref<document_identifier> r_doc)
{
    boole rst;
    rw_lock_wait_write(_op_lock);
    if (document_map.count(document_id) == 0)
    {
        document_map[document_id] = r_doc;
        rst = boole::True;
    }
    else
    {
        rst = boole::False;
    }
    rw_lock_put_write(_op_lock);
    return rst;
}

boole index_document_id::remove_document(const string& document_id)
{
    boole rst = boole::False;
    rw_lock_wait_write(_op_lock);
    if (document_map.count(document_id))
    {
        if (document_map.erase(document_id))
        {
            rst = boole::True;
        }
    }
    rw_lock_put_write(_op_lock);
    return rst;
}

set<string> index_table::get_documents(ref<json_base> key)
{
    set<string> rst;
    rw_lock_wait_read(_op_lock);
    if (document_map.count(key))
    {
        rst = document_map[key];
    }
    rw_lock_put_read(_op_lock);
    return rst;
}

boole index_table::insert_document(ref<json_base> key, const string& document_id)
{
    auto new_doc = ref<document_identifier>::new_instance(index_string, _partition_location);
    ref<json_base> r_json_document_id = ref<json_string>::new_instance(index_string).ref_of<json_base>();
    ref<document_identifier> rst;
    rw_lock_wait_write(_op_lock);
    if (document_map.count(r_json_document_id) == 0)
    {
        document_map[r_json_document_id] = new_doc;
        rst = new_doc;
    }
    else
    {
        rst = document_map[document_id];
    }
    rw_lock_put_write(_op_lock);
    return rst;
}

boole index_table::remove_document(ref<json_base> key, const string& document_id)
{
    rw_lock_wait_write(_op_lock);
    document_map.erase(document_id);
    rw_lock_put_write(_op_lock);
}

}
}
