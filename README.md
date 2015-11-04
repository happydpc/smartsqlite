SmartSqlite
=============

SmartSqlite is a C++11 wrapper around SQLite3.
We call it "smart" because of the consistent use of smart pointers.

A copy of SQLite is shipped with this library,
thanks [Richard](https://www.sqlite.org/crew.html)!


Get started
-----------

```
$ git clone --recursive https://github.com/kullo/smartsqlite.git
$ cmake .
$ make
$ ctest
```


Versions
----------------

SmartSqlite has a linear versioning and the API might break at any time.

Version     | SQLite version | Changes in SmartSqlite
----------- | -------------- | ----------------------
v1          | 3.9.2          | Initial version


Compiler support
----------------

* GCC 4.8+
* Clang 3.4+
* MSVC 2013+


Contibutions
----------------

Contibutions respecting the library's license
are highly welcome via Github issues and pull requests.


Users
----------------

This library is used by:

* [Kullo](https://www.kullo.net/) (Windows, Linux, OS X, Android and iOS)

Please add yourself to the list using PRs.


License
-------

```
Copyright 2014–2015 Daniel Seither (Kullo GmbH)
          2014–2015 Simon Warta (Kullo GmbH)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
