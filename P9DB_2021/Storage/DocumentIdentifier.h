#pragma once

namespace P9
{

namespace Storage
{

class document_identifier
{
public:
    string id;
    string location;
    lock   op_lock;
};

}
}
