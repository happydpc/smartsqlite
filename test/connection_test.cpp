#include <memory>
#include <gmock/gmock.h>
#include <type_traits>

#include "sqlitewrapper/connection.h"
#include "sqlitewrapper/exceptions.h"

using namespace testing;

TEST(Connection, canMove)
{
    EXPECT_THAT((std::is_move_constructible<SqliteWrapper::Connection>::value),
                Eq(true));
    EXPECT_THAT((std::is_move_assignable<SqliteWrapper::Connection>::value),
                Eq(true));
}

TEST(Connection, cannotCopy)
{
    EXPECT_THAT((std::is_copy_constructible<SqliteWrapper::Connection>::value),
                Eq(false));
    EXPECT_THAT((std::is_copy_assignable<SqliteWrapper::Connection>::value),
                Eq(false));
}

TEST(Connection, cannotOpenFileInNonexistingFolder)
{
    EXPECT_THROW(SqliteWrapper::Connection("/does/not/exist"),
                 SqliteWrapper::Exception);
}

TEST(Connection, canBeOpened)
{
    SqliteWrapper::Connection(":memory:");
}

TEST(Connection, canSetBusyTimeout)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.setBusyTimeout(42);
}

TEST(Connection, canPrepareStatement)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.prepare("PRAGMA user_version");
}

TEST(Connection, canExec)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.exec("PRAGMA user_version = 42");
}

TEST(Connection, canBeginTransaction)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.beginTransaction();
}

TEST(Connection, canCommitTransaction)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.beginTransaction();
    conn.commitTransaction();
}

TEST(Connection, canRollbackTransaction)
{
    SqliteWrapper::Connection conn(":memory:");
    conn.beginTransaction();
    conn.rollbackTransaction();
}
