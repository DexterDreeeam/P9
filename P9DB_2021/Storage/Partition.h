#pragma once

namespace P9
{

namespace Storage
{

class partition
{
public:
    partition(const string& name, const string& location) :
        _name(name),
        _location(location),
        _op_lock(lock_create())
    {}

    ~partition()
    {
        for (auto& p : table_map)
        {
            delete p.second;
        }
        lock_destroy(_op_lock);
    }

    index_table* wait_read(const string& index_table_name)
    {
        index_table* rst = nullptr;
        atom_increment(_readers);
        lock_wait_get(_op_lock);
        if (table_map.count(index_table_name))
        {
            rst = table_map[index_table_name];
        }
        lock_put(_op_lock);
    }

private:
    string       _name;
    string       _location;
    lock         _op_lock;
    volatile s64 _readers;

private:
    unordered_map<string, index_table*> table_map;
};

}
}
