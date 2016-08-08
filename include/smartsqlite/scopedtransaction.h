/*
 * Copyright 2014–2016 The SmartSqlite contributors (see NOTICE.txt)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <memory>

#include "smartsqlite/connection.h"

namespace SmartSqlite {

class ScopedTransaction final
{
public:
    explicit ScopedTransaction(
            std::shared_ptr<Connection> conn,
            TransactionType type = Deferred);
    ScopedTransaction(ScopedTransaction &&other);
    ScopedTransaction &operator=(ScopedTransaction &&rhs);
    ~ScopedTransaction();
    void commit();
    void rollback();

private:
    // ScopedTransaction is not copyable
    ScopedTransaction(const ScopedTransaction &) = delete;
    ScopedTransaction &operator=(const ScopedTransaction &) = delete;

    void rollbackIfNotFinished();

    std::shared_ptr<Connection> conn_;
    bool finished_ = false;
};

}
