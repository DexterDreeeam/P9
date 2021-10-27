#pragma once

namespace P9
{

namespace Storage
{

class storage : object
{
public:
    storage() = delete;

    storage(const string& platform_location) :
        _location(platform_location + "storage" + '/'),
        _op_lock(rw_lock_create())
    {}

    ~storage()
    {
        rw_lock_destroy(_op_lock);
    }

    ref<partition> get_partition(const string& partition_name);

    ref<partition> insert_partition(const string& partition_name);

    void remove_partition(const string& partition_name);

private:
    string       _location;
    rw_lock      _op_lock;

private:
    map<string, ref<partition>> partition_map;
};

}
}
