#pragma once

class map_comparer
{
public:
    s64 operator()(const void* const v1, const void* const v2)
    {
        auto r1 = (u64)v1;
        auto r2 = (u64)v2;
        if (r1 == r2)
        {
            return 0;
        }
        else if (r1 < r2)
        {
            return -1;
        }
        else if (r1 > r2)
        {
            return 1;
        }
        else
        {
            assert(0);
            return 0;
        }
    }

    template<typename Ty>
    s64 operator()(const ref<Ty>& v1, const ref<Ty>& v2)
    {
        auto r1 = (u64)v1.raw_ptr();
        auto r2 = (u64)v2.raw_ptr();
        if (r1 == r2)
        {
            return 0;
        }
        else if (r1 < r2)
        {
            return -1;
        }
        else if (r1 > r2)
        {
            return 1;
        }
        else
        {
            assert(0);
            return 0;
        }
    }

    template<typename Ty>
    s64 operator()(const obs<Ty>& v1, const obs<Ty>& v2)
    {
        auto r1 = (u64)v1.raw_ptr();
        auto r2 = (u64)v2.raw_ptr();
        if (r1 == r2)
        {
            return 0;
        }
        else if (r1 < r2)
        {
            return -1;
        }
        else if (r1 > r2)
        {
            return 1;
        }
        else
        {
            assert(0);
            return 0;
        }
    }

    template<typename Ty>
    s64 operator ()(const Ty& v1, const Ty& v2)
    {
        if (v1 == v2)
        {
            return 0;
        }
        else if (v1 < v2)
        {
            return -1;
        }
        else if (v1 > v2)
        {
            return 1;
        }
        else
        {
            assert(0);
            return 0;
        }
    }
};

template<typename Key_Ty, typename Val_Ty, typename Cmp_Ty = map_comparer>
class map : public _Internal::_RbTree::rbtree<Key_Ty, Val_Ty, Cmp_Ty>
{
    using Self_Ty = map<Key_Ty, Val_Ty>;
    using Base_Ty = _Internal::_RbTree::rbtree<Key_Ty, Val_Ty, Cmp_Ty>;
    using Node_Ty = typename Base_Ty::Node_Ty;

    using Iter_Ty = typename Base_Ty::Iter_Ty;
    using Ritr_Ty = typename Base_Ty::Ritr_Ty;
    using cIter_Ty = typename Base_Ty::cIter_Ty;
    using cRitr_Ty = typename Base_Ty::cRitr_Ty;

public:
    map() :
        Base_Ty()
    {}

    map(const Self_Ty& rhs) :
        Base_Ty()
    {
        for (const auto& k : rhs)
        {
            this->insert(k);
        }
    }

    ~map() noexcept
    {}

    void insert(const Key_Ty& k)
    {
        Base_Ty::_insert(k);
    }

    void insert(const Key_Ty& k, const Val_Ty& v)
    {
        Base_Ty::_insert(k, v);
    }

    void insert(const Key_Ty& k, Val_Ty&& v)
    {
        Base_Ty::_insert(k, v);
    }

    void insert(const pair<Key_Ty, Val_Ty>& p)
    {
        Base_Ty::_insert(p.first, p.second);
    }

    void insert(pair<Key_Ty, Val_Ty>&& p)
    {
        Base_Ty::_insert(right_value_type(p.first), right_value_type(p.second));
    }

    s64 size() const noexcept
    {
        return Base_Ty::_size();
    }

    s64 count(const Key_Ty& k) const
    {
        return Base_Ty::_peek_node(k) ? 1 : 0;
    }

    void clear() noexcept
    {
        Base_Ty::_clear();
    }

    Val_Ty& operator [](const Key_Ty& k)
    {
        Node_Ty* nod = Base_Ty::_insert_node(k);
        assert(nod);
        return nod->mydata()->second;
    }

    Iter_Ty find(const Key_Ty& k)
    {
        Node_Ty* nod = Base_Ty::_peek_node(k);
        if (nod)
        {
            return Iter_Ty(nod);
        }
        else
        {
            return end();
        }
    }

    bool erase(const Key_Ty& k)
    {
        return Base_Ty::_erase(k);
    }

    void erase(Iter_Ty itr) noexcept
    {
        assert_info(itr.node->prnt, "Cannot erase no data iterator");
        Node_Ty* nod = (Node_Ty*)itr.node;
        Base_Ty::_remove_node(nod);
    }

    void erase(Ritr_Ty itr) noexcept
    {
        assert_info(itr.node->prnt, "Cannot erase no data iterator");
        Node_Ty* nod = (Node_Ty*)itr.node;
        Base_Ty::_remove_node(nod);
    }

    Iter_Ty begin() const noexcept
    {
        Iter_Ty first((Node_Ty*)Base_Ty::myhead());
        ++first;
        return first;
    }

    Iter_Ty end() const noexcept
    {
        return Iter_Ty((Node_Ty*)Base_Ty::myhead());
    }

    Ritr_Ty rbegin() const noexcept
    {
        Ritr_Ty first((Node_Ty*)Base_Ty::myhead());
        ++first;
        return first;
    }

    Ritr_Ty rend() const noexcept
    {
        return Ritr_Ty((Node_Ty*)Base_Ty::myhead());
    }

    cIter_Ty cbegin() const noexcept
    {
        cIter_Ty first((Node_Ty*)Base_Ty::myhead());
        ++first;
        return first;
    }

    cIter_Ty cend() const noexcept
    {
        return cIter_Ty((Node_Ty*)Base_Ty::myhead());
    }

    cRitr_Ty crbegin() const noexcept
    {
        cRitr_Ty first((Node_Ty*)Base_Ty::myhead());
        ++first;
        return first;
    }

    cRitr_Ty crend() const noexcept
    {
        return cRitr_Ty((Node_Ty*)Base_Ty::myhead());
    }
};
