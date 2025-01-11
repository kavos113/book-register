cmake_minimum_required(VERSION 3.30)
project(book_register)

set(CMAKE_CXX_STANDARD 23)

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
    view/util.h
    view/View.cpp
    view/View.h
    main.cpp)
target_compile_options(book_register PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
target_link_libraries(book_register PRIVATE CURL::libcurl)