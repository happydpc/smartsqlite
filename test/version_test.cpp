/*
 * Copyright 2014–2017 The SmartSqlite contributors (see NOTICE.txt)
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
#include <gmock/gmock.h>

#include "smartsqlite/version.h"

using namespace testing;

TEST(Version, returnsVersion)
{
    std::string version = SmartSqlite::version();
    EXPECT_THAT(version, Not(IsEmpty()));
}

TEST(Version, returnsVersionCode)
{
    auto version = SmartSqlite::versionCode();
    EXPECT_THAT(version, Gt(0));
}

TEST(Version, versionEqualsVersionCode)
{
    auto version = SmartSqlite::version();
    auto fromVersionCode = "v" + std::to_string(SmartSqlite::versionCode());
    EXPECT_THAT(version, StrEq(fromVersionCode));
}

TEST(Version, returnsSqliteVersion)
{
    std::string version = SmartSqlite::sqliteVersion();
    EXPECT_THAT(version, Not(IsEmpty()));
}

TEST(Version, sqliteVersionCheckIsOk)
{
    EXPECT_THAT(SmartSqlite::checkSqliteVersion(), Eq(true));
}
