#include "../../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<partition> storage::get_partition(const string& partition_name)
{
    AUTO_TRACE;

    ref<partition> rst;
    _op_lock.wait_read();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.read_release();
        };

    if (partition_map.count(partition_name))
    {
        rst = partition_map[partition_name];
    }
    return rst;
}

ref<partition> storage::insert_partition(const string& partition_name)
{
    AUTO_TRACE;

    auto new_table = ref<partition>::new_instance(
        partition_name, _location);
    ref<partition> rst;
    _op_lock.wait_write();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.write_release();
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
    AUTO_TRACE;

    _op_lock.wait_write();
    escape_function ef =
        [=]() mutable
        {
            _op_lock.write_release();
        };

    partition_map.erase(partition_name);
}

}
}
