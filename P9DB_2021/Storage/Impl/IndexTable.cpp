#include "../Interface.h"

namespace P9
{

namespace Storage
{

ref<document_identifier> index_table::get_document(const string& document_id)
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

ref<document_identifier> index_table::insert_document(const string& document_id)
{
    auto new_table = ref<document_identifier>::new_instance(document_id, _location);
    ref<document_identifier> rst;
    rw_lock_wait_write(_op_lock);
    if (document_map.count(document_id) == 0)
    {
        document_map[document_id] = new_table;
        rst = new_table;
    }
    else
    {
        rst = document_map[document_id];
    }
    rw_lock_put_write(_op_lock);
    return rst;
}

void index_table::insert_index(ref<document_identifier> r)
{

}

void index_table::remove_document(const string& document_id)
{
    rw_lock_wait_write(_op_lock);
    document_map.erase(document_id);
    rw_lock_put_write(_op_lock);
}

}
}
