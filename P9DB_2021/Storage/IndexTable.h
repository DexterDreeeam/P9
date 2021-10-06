#pragma once

namespace P9
{

namespace Storage
{

class index_table
{
public:
    string name;
    string location;
    lock   op_lock;

private:
    map<string, document_identifier> document_map;
};

}
}
