#pragma once

#include "../../../Sdk/_Interface.hpp"
#include "../Vec/Vec.hpp"

namespace gpx
{

enum class mat_major : s64
{
    Row,
    Col,
};

template<s64 Row, s64 Col, typename ElemTy = f32, mat_major Major = mat_major::Row>
class mat;

template<s64 Row, s64 Col, typename ElemTy>
class mat<Row, Col, ElemTy, mat_major::Row>
{
public:
    static const mat_major MajorTy = mat_major::Row;

    using SelfTy = mat<Row, Col, ElemTy, MajorTy>;
    using TransposeTy = mat<Col, Row, ElemTy, MajorTy>;
    using RowTy = vec<Col, ElemTy>;
    using ColTy = vec<Row, ElemTy>;

    static const s64 _total_count = Row * Col;

public:
    mat() :
        rows()
    {
    }

    mat(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Row; ++i)
        {
            rows[i] = rhs.rows[i];
        }
    }

    template<typename ...Args>
    mat(ElemTy elem, Args ...args) :
        mat()
    {
        _construct(0, elem, args...);
    }

    template<typename ...Args>
    mat(const RowTy& v, Args ...args) :
        mat()
    {
        _construct(0, v, args...);
    }

    SelfTy& operator =(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Row; ++i)
        {
            rows[i] = rhs.rows[i];
        }
        return *this;
    }

    ~mat() = default;

public:
    RowTy& operator [](s64 vi)
    {
        assert(vi >= 0 && vi < Row);
        return rows[vi];
    }

    const RowTy& operator [](s64 vi) const
    {
        assert(vi >= 0 && vi < Row);
        return rows[vi];
    }

    RowTy row_at(s64 vi) const
    {
        assert(vi >= 0 && vi < Row);
        return rows[vi];
    }

    ColTy col_at(s64 ci) const
    {
        assert(ci >= 0 && ci < Col);
        ColTy cv;
        for (s64 r = 0; r < Row; ++r)
        {
            cv[r] = rows[r][ci];
        }
        return cv;
    }

    ElemTy& at(s64 r, s64 c)
    {
        assert(r >= 0 && r < Row);
        assert(c >= 0 && c < Col);
        return rows[r][c];
    }

public:
    static SelfTy identity()
    {
        SelfTy m;
        s64 i = 0;
        while (i < Row && i < Col)
        {
            m[i][i] = 1;
            ++i;
        }
        return m;
    }

public:
    SelfTy& operator +=(const SelfTy& rhs)
    {
        for (s64 r = 0; r < Row; ++r)
        {
            rows[r] += rhs.rows[r];
        }
        return *this;
    }

    SelfTy operator +(const SelfTy& rhs) const
    {
        SelfTy m(*this);
        m += rhs;
        return m;
    }

    SelfTy& operator -=(const SelfTy& rhs)
    {
        for (s64 r = 0; r < Row; ++r)
        {
            rows[r] -= rhs.rows[r];
        }
        return *this;
    }

    SelfTy operator -(const SelfTy& rhs) const
    {
        SelfTy m(*this);
        m -= rhs;
        return m;
    }

public:
    template<s64 NewCol>
    auto dot(const mat<Col, NewCol, ElemTy, MajorTy>& rhs) const
        -> mat<Row, NewCol, ElemTy, MajorTy>
    {
        using RstMatTy = mat<Row, NewCol, ElemTy, MajorTy>;
        using RstRowTy = typename RstMatTy::RowTy;
        using RhsMatTy = mat<Col, NewCol, ElemTy, MajorTy>;
        using RhsColTy = typename RhsMatTy::ColTy;

        RstMatTy rst;
        RhsColTy RhsCols[NewCol];
        for (s64 c = 0; c < NewCol; ++c)
        {
            RhsCols[c] = rhs.col_at(c);
        }
        for (s64 r = 0; r < Row; ++r)
        {
            RstRowTy new_row;
            for (s64 c = 0; c < NewCol; ++c)
            {
                new_row.at(c) = row_at(r) * RhsCols[c];
            }
            rst.rows[r] = new_row;
        }
        return rst;
    }

    template<s64 NewCol>
    auto operator *(const mat<Col, NewCol, ElemTy, MajorTy>& rhs) const
        -> mat<Row, NewCol, ElemTy, MajorTy>
    {
        return dot(rhs);
    }

public:
    TransposeTy transpose() const
    {
        TransposeTy t;
        for (s64 r = 0; r < Row; ++r)
        {
            for (s64 c = 0; c < Col; ++c)
            {
                t.at(c, r) = at(r, c);
            }
        }
        return t;
    }

private:
    template<typename ...Args>
    void _construct(s64 pos, ElemTy elem, Args ...args)
    {
        assert(pos < _total_count);

        rows[pos / Col][pos % Col] = elem;
        _construct(pos + 1, args...);
    }

    void _construct(s64 pos, ElemTy elem)
    {
        assert(pos < _total_count);

        rows[pos / Col][pos % Col] = elem;
    }

    template<typename ...Args>
    void _construct(s64 pos, RowTy v, Args ...args)
    {
        assert(pos % Col == 0);
        assert(pos < _total_count);

        rows[pos / Col] = v;
        _construct(pos + Col, args...);
    }

    void _construct(s64 pos, RowTy v)
    {
        assert(pos % Col == 0);
        assert(pos < _total_count);

        rows[pos / Col] = v;
    }

public:
    RowTy rows[Row];
};

template<s64 Row, s64 Col, typename ElemTy>
class mat<Row, Col, ElemTy, mat_major::Col>
{
public:
    static const mat_major MajorTy = mat_major::Col;

    using SelfTy = mat<Row, Col, ElemTy, MajorTy>;
    using TransposeTy = mat<Col, Row, ElemTy, MajorTy>;
    using RowTy = vec<Col, ElemTy>;
    using ColTy = vec<Row, ElemTy>;

    static const s64 _total_count = Row * Col;

public:
    mat() :
        cols()
    {
    }

    mat(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Col; ++i)
        {
            cols[i] = rhs.cols[i];
        }
    }

    template<typename ...Args>
    mat(ElemTy elem, Args ...args) :
        mat()
    {
        _construct(0, elem, args...);
    }

    template<typename ...Args>
    mat(const ColTy& v, Args ...args) :
        mat()
    {
        _construct(0, v, args...);
    }

    SelfTy& operator =(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Col; ++i)
        {
            cols[i] = rhs.cols[i];
        }
        return *this;
    }

    ~mat() = default;

public:
    ColTy& operator [](s64 ci)
    {
        assert(ci >= 0 && ci < Col);
        return cols[ci];
    }

    const ColTy& operator [](s64 ci) const
    {
        assert(ci >= 0 && ci < Col);
        return cols[ci];
    }

    RowTy row_at(s64 vi) const
    {
        assert(vi >= 0 && vi < Row);
        RowTy rv;
        for (s64 c = 0; c < Col; ++c)
        {
            rv[c] = cols[c][vi];
        }
        return rv;
    }

    ColTy col_at(s64 ci) const
    {
        assert(ci >= 0 && ci < Col);
        return cols[ci];
    }

    ElemTy& at(s64 r, s64 c)
    {
        assert(r >= 0 && r < Row);
        assert(c >= 0 && c < Col);
        return cols[c][r];
    }

public:
    static SelfTy identity()
    {
        SelfTy m;
        s64 i = 0;
        while (i < Row && i < Col)
        {
            m[i][i] = 1;
            ++i;
        }
        return m;
    }

public:
    SelfTy& operator +=(const SelfTy& rhs)
    {
        for (s64 c = 0; c < Col; ++c)
        {
            cols[c] += rhs.cols[c];
        }
        return *this;
    }

    SelfTy operator +(const SelfTy& rhs) const
    {
        SelfTy m(*this);
        m += rhs;
        return m;
    }

    SelfTy& operator -=(const SelfTy& rhs)
    {
        for (s64 c = 0; c < Col; ++c)
        {
            cols[c] -= rhs.cols[c];
        }
        return *this;
    }

    SelfTy operator -(const SelfTy& rhs) const
    {
        SelfTy m(*this);
        m -= rhs;
        return m;
    }

public:
    template<s64 NewCol>
    auto dot(const mat<Col, NewCol, ElemTy, MajorTy>& rhs) const
        -> mat<Row, NewCol, ElemTy, MajorTy>
    {
        using RstMatTy = mat<Row, NewCol, ElemTy, MajorTy>;
        using RstColTy = typename RstMatTy::ColTy;
        using LhsRowTy = RowTy;

        RstMatTy rst;
        LhsRowTy LhsRows[Row];
        for (s64 r = 0; r < Row; ++r)
        {
            LhsRows[r] = row_at(r);
        }
        for (s64 c = 0; c < NewCol; ++c)
        {
            RstColTy new_col;
            for (s64 r = 0; r < Row; ++r)
            {
                new_col.at(r) = LhsRows[r] * rhs.cols[c];
            }
            rst.cols[c] = new_col;
        }
        return rst;
    }

    template<s64 NewCol>
    auto operator *(const mat<Col, NewCol, ElemTy, MajorTy>& rhs) const
        -> mat<Row, NewCol, ElemTy, MajorTy>
    {
        return dot(rhs);
    }

public:
    TransposeTy transpose() const
    {
        TransposeTy t;
        for (s64 r = 0; r < Row; ++r)
        {
            for (s64 c = 0; c < Col; ++c)
            {
                t.at(c, r) = at(r, c);
            }
        }
        return t;
    }

private:
    template<typename ...Args>
    void _construct(s64 pos, ElemTy elem, Args ...args)
    {
        assert(pos < _total_count);

        cols[pos / Row][pos % Row] = elem;
        _construct(pos + 1, args...);
    }

    void _construct(s64 pos, ElemTy elem)
    {
        assert(pos < _total_count);

        cols[pos / Row][pos % Row] = elem;
    }

    template<typename ...Args>
    void _construct(s64 pos, ColTy v, Args ...args)
    {
        assert(pos % Row == 0);
        assert(pos < _total_count);

        cols[pos / Row] = v;
        _construct(pos + Row, args...);
    }

    void _construct(s64 pos, ColTy v)
    {
        assert(pos % Row == 0);
        assert(pos < _total_count);

        cols[pos / Row] = v;
    }

public:
    ColTy cols[Col];
};

}
