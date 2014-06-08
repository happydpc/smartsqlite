#ifndef SQLITEWRAPPER_SQLITEWRAPPER_H
#define SQLITEWRAPPER_SQLITEWRAPPER_H

#include <memory>
#include <string>

#include "statement.h"

namespace SqliteWrapper {

class Connection
{
public:
    explicit Connection(const std::string &connectionString);
    Connection(Connection &&other);
    ~Connection();

    Statement prepare(const std::string &sql);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}
#endif // SQLITEWRAPPER_SQLITEWRAPPER_H
