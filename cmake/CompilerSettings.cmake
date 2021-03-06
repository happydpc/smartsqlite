if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Use libc++ on Linux and OS X; use libstdc++ on Android.
    # Clang on Linux defaults to libstdc++.
    if(NOT ANDROID)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()

    set(warn_flags "-Qunused-arguments -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-braces -Wno-padded -Wno-weak-vtables")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread ${warn_flags}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ${warn_flags}")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(warn_flags "-Wall -Wextra -Wpedantic -Wcast-qual -Wformat=2 -Wlogical-op -Wmissing-include-dirs -Wswitch-default -Wno-maybe-uninitialized")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread ${warn_flags}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ${warn_flags} -Woverloaded-virtual")

# Workaround for CMake < 3.1: Variables such as 'MSVC' are expanded in 'if'
# statements even if they are quoted. We can't enable the policy CMP0054 because
# we need to support CMake 2.8. Therefore, we append a space on both sides,
# which disables automatic expansion.
elseif("${CMAKE_CXX_COMPILER_ID} " STREQUAL "MSVC ")
    # force gtest to use shared MSVC++ runtime
    set(gtest_force_shared_crt ON)
endif()

if(APPLE)
    if(NOT IOS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.9")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.9")
    endif()

# non-Apple unices
elseif(UNIX)
    # Hardening
    set(hardening_cflags  "-fstack-protector -fPIC -D_FORTIFY_SOURCE=2")
    set(hardening_ldflags "-Wl,-z,relro,-z,now")

    set(CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE}          ${hardening_cflags}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO}   ${hardening_cflags}")
    set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}        ${hardening_cflags}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${hardening_cflags}")

    set(CMAKE_EXE_LINKER_FLAGS_RELEASE           "${CMAKE_EXE_LINKER_FLAGS_RELEASE}           -pie ${hardening_ldflags}")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO    "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}    -pie ${hardening_ldflags}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE        "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}        -pic ${hardening_ldflags}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} -pic ${hardening_ldflags}")
endif()
