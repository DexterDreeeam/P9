#pragma once

namespace P9
{

namespace Storage
{

class partition
{
public:
    partition() = delete;

    partition(const string& name, const string& location) :
        _name(name),
        _location(location),
        _op_lock(rw_lock_create())
    {}

    ~partition()
    {
        rw_lock_destroy(_op_lock);
    }

    ref<index_table> get_index_table(const string& index_table_name);

    ref<index_table> insert_index_table(const string& index_table_name);

    void remove_index_table(const string& index_table_name);

private:
    string       _name;
    string       _location;
    rw_lock      _op_lock;

private:
    map<string, ref<index_table>> table_map;
};

}
}
