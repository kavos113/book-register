cmake_minimum_required(VERSION 3.30)
project(book_register)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(controller)
include_directories(network)
include_directories(view)

find_package(CURL REQUIRED)

add_executable(book_register WIN32
        controller/Book.h
        network/Client.cpp
        network/Client.h
        view/Application.cpp
        view/Application.h
        view/Detail.cpp
        view/Detail.h
        view/DXFactory.cpp
        view/DXFactory.h
        view/Search.cpp
        view/Search.h
        view/Table.cpp
        view/Table.h
        util.h
        view/View.cpp
        view/View.h
        main.cpp
        db/Parser.cpp
        db/Parser.h
        db/Database.cpp
        db/Database.h
        view/resource.h
        view/app.rc
        view/AddDialog.cpp
        view/AddDialog.h)

target_compile_options(book_register PRIVATE /source-charset:utf-8 /execution-charset:utf-8)

target_link_libraries(book_register PRIVATE CURL::libcurl)
target_include_directories(book_register PRIVATE $ENV{xercesc_dir}/src)
target_include_directories(book_register PRIVATE $ENV{xercesc_dir}/build/src)
target_link_libraries(book_register PRIVATE $ENV{xercesc_dir}/build/src/Debug/xerces-c_3D.lib)
target_include_directories(book_register PRIVATE $ENV{libsqlite3_dir})
target_link_libraries(book_register PRIVATE $ENV{libsqlite3_dir}/sqlite3.lib)
file(COPY $ENV{libsqlite3_dir}/sqlite3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
