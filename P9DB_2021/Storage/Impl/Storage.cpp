#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<partition> storage::get_partition(const string& partition_name)
{
    ref<partition> rst;
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    if (partition_map.count(partition_name))
    {
        rst = partition_map[partition_name];
    }
    return rst;
}

ref<partition> storage::insert_partition(const string& partition_name)
{
    auto new_table = ref<partition>::new_instance(
        partition_name, _location);
    ref<partition> rst;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    if (partition_map.count(partition_name) == 0)
    {
        partition_map[partition_name] = new_table;
        rst = new_table;
    }
    else
    {
        rst = partition_map[partition_name];
    }
    return rst;
}

void storage::remove_partition(const string& partition_name)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    partition_map.erase(partition_name);
}

}
}
