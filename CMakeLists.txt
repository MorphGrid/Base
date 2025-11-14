cmake_minimum_required(VERSION 3.30)
project(base VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build framework tests cases" OFF)
option(BUILD_PROGRAM_TESTS "Build program tests cases" ON)
option(ENABLE_DEBUG "Enable debugging" ON)
option(ENABLE_STATIC_LINKING "Enable static linking feature" OFF)
option(ENABLE_NATIVE_OPTIMIZATION "Enable native CPU optimization feature" OFF)
option(ENABLE_CI "Enable CI settings" OFF)

if (ENABLE_DEBUG)
    add_definitions(-DDEBUG_ENABLED)
endif ()


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


if (ENABLE_STATIC_LINKING)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    set(BOOST_ALL_NO_LIB ON)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    if (ENABLE_CI)
        set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/libcrypto.a")
        set(OPENSSL_SSL_LIBRARY "/usr/lib/libssl.a")
    else ()
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
            set(ARCH_LIB_DIR "/usr/lib/aarch64-linux-gnu")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            set(ARCH_LIB_DIR "/usr/lib/x86_64-linux-gnu")
        else()
            message(FATAL_ERROR "Arquitectura no soportada: ${CMAKE_SYSTEM_PROCESSOR}")
        endif()
        set(OPENSSL_CRYPTO_LIBRARY "${ARCH_LIB_DIR}/libcrypto.a")
        set(OPENSSL_SSL_LIBRARY "${ARCH_LIB_DIR}/libssl.a")
    endif ()
endif ()

if (ENABLE_STATIC_LINKING)
    set(OpenSSL_USE_STATIC_LIBS TRUE)

    if (ENABLE_NATIVE_OPTIMIZATION)
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -march=native -mtune=native -g0 -static -static-libgcc -DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -march=native -mtune=native -g0 -static -static-libgcc -DNDEBUG")
    else ()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -g0 -static -static-libgcc -DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -g0 -static -static-libgcc -DNDEBUG")
    endif ()
else ()
    if (ENABLE_NATIVE_OPTIMIZATION)
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -march=native -mtune=native -g0 -DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -march=native -mtune=native -g0 -DNDEBUG")
    else ()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    endif ()
endif ()

include(FetchContent)


find_package(Boost REQUIRED COMPONENTS program_options json charconv)
find_package(OpenSSL REQUIRED)
find_package(laserpants_dotenv REQUIRED)

find_package(framework)
include_directories(src/include)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/objects/*.cxx")

if (BUILD_PROGRAM_TESTS)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc")

    add_executable(tests ${TESTS} ${SOURCES})
    if(ENABLE_STATIC_LINKING)
        target_compile_options(tests PRIVATE -static -static-libgcc -static-libstdc++)
        target_link_options(tests PRIVATE -static -static-libgcc -static-libstdc++)
    endif ()

    target_link_libraries(tests GTest::gtest_main framework
            Boost::program_options
            Boost::json
            Boost::charconv
            laserpants::dotenv
            ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY}
            bcrypt
    )

    include(GoogleTest)
    gtest_discover_tests(tests)
endif ()

add_executable(base main.cpp ${SOURCES})
target_link_libraries(base PUBLIC framework
        Boost::program_options
        Boost::json
        Boost::charconv
        laserpants::dotenv
        ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY}
        bcrypt
)

if (ENABLE_STATIC_LINKING)
    target_compile_options(base PRIVATE -static -static-libgcc -static-libstdc++)
    target_link_options(base PRIVATE -static -static-libgcc -static-libstdc++)
endif ()