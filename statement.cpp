#include "statement.h"

#include "sqlite3.h"
#include "util.h"

namespace SqliteWrapper {

struct Statement::Impl
{
    sqlite3_stmt *stmt;
};

Statement::Statement(sqlite3_stmt *stmt)
    : impl(new Impl)
{
    impl->stmt = stmt;
}

Statement::Statement(Statement &&other)
    : impl(new Impl)
{
    std::swap(impl, other.impl);
}

Statement::~Statement()
{
    sqlite3_finalize(impl->stmt);
}

void Statement::clearBindings()
{
    checkResult(sqlite3_clear_bindings(impl->stmt));
}

void Statement::reset()
{
    checkResult(sqlite3_reset(impl->stmt));
}

}
