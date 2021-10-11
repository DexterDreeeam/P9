#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<index_table> partition::get_index_table(const string& index_table_name)
{
    ref<index_table> rst;
    rw_lock_wait_read(_op_lock);
    if (_index_table_map.count(index_table_name))
    {
        rst = _index_table_map[index_table_name];
    }
    rw_lock_put_read(_op_lock);
    return rst;
}

ref<index_table> partition::insert_index_table(const string& index_table_name)
{
    auto new_table = ref<index_table>::new_instance();
    ref<index_table> rst;
    rw_lock_wait_write(_op_lock);
    if (_index_table_map.count(index_table_name) == 0)
    {
        _index_table_map[index_table_name] = new_table;
        rst = new_table;
    }
    else
    {
        rst = _index_table_map[index_table_name];
    }
    rw_lock_put_write(_op_lock);
    return rst;
}

void partition::remove_index_table(const string& index_table_name)
{
    rw_lock_wait_write(_op_lock);
    _index_table_map.erase(index_table_name);
    rw_lock_put_write(_op_lock);
}

}
}
