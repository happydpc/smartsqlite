#ifndef SQLITEWRAPPER_STATEMENT_H
#define SQLITEWRAPPER_STATEMENT_H

#include <iterator>
#include <memory>

#include "util.h"

struct sqlite3_stmt;

namespace SqliteWrapper {

class RowIterator;

class Row
{
public:
    Row(sqlite3_stmt *stmt);

    template <typename T>
    T get(int pos)
    {
        checkPosRange(pos);
        return getUnchecked<T>(pos);
    }

    // specialize this to add bindings for custom types
    template <typename T>
    T getUnchecked(int pos);

private:
    void setColumns(int columns);
    void checkPosRange(int pos);

    sqlite3_stmt *m_stmt = nullptr;
    int m_columns = 0;

    friend class RowIterator;
};

class RowIterator : public std::iterator<std::input_iterator_tag, Row, void>
{
public:
    RowIterator(sqlite3_stmt *stmt, bool done);
    bool operator==(const RowIterator &rhs) const;
    bool operator!=(const RowIterator &rhs) const;
    RowIterator &operator++();
    Row &operator*();
    Row *operator->();

private:
    sqlite3_stmt *m_stmt;
    bool m_done;
    Row m_row;
};

class Statement
{
public:
    explicit Statement(sqlite3_stmt *stmt);
    Statement(Statement &&other);
    Statement &operator=(Statement &&rhs);
    ~Statement();

    template <typename... T>
    Statement &bind(int pos, const T&... values)
    {
        checkResult(bindUnchecked(pos, values...));
        return *this;
    }

    // specialize this to add bindings for custom types
    template <typename... T>
    int bindUnchecked(int pos, const T&... values);

    Statement &bindNull(int pos);
    int bindUncheckedNull(int pos);

    RowIterator begin();
    RowIterator end();

    void clearBindings();
    void reset();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}
#endif // SQLITEWRAPPER_STATEMENT_H
