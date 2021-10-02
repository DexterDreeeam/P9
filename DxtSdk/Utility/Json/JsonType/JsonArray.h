#pragma once

class json_array : public JsonNs::json_base
{
public:
    json_array() :
        _items()
    {
    }

    json_array(const json_array& rhs) :
        _items()
    {
        for (const auto& item : rhs._items)
        {
            _items.push_back(item->clone());
        }
    }

    template<typename ...Args>
    json_array(Args ...args) :
        _items()
    {
        constructor_iterator(args...);
    }

    ~json_array()
    {
        for (auto* item : _items)
        {
            destroy(item);
        }
    }

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::ARRAY;
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_array(*this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_array::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += '[';
        boole not_first = boole::False;
        for (auto item : _items)
        {
            if (not_first)
            {
                str += ',';
            }
            else
            {
                not_first = boole::True;
            }
            item->serialize_append(str);
        }
        str += ']';
    }

    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        return nullptr;
    }

private:
    template<typename ...Args>
    void constructor_iterator(const json_base& jo, Args ...args)
    {
        _items.push_back(jo.clone());
        constructor_iterator(args...);
    }

    void constructor_iterator(const json_base& jo)
    {
        _items.push_back(jo.clone());
    }

private:
    vector<json_base*> _items;
};
