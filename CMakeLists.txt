cmake_minimum_required(VERSION 3.15)
project(mini_sql)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(mini_sql
        src/Attribute.h
        src/BufferManager.cpp
        src/BufferManager.h
        src/CatalogManager.cpp
        src/CatalogManager.h
        src/Condition.h
        src/Condition.cpp
        src/interpreter.cpp
        src/interpreter.h
        src/minisql.h
        src/RecordManager.h
        src/RecordManager.cpp
        src/testing.cpp
        src/api.h
        src/api.cpp
        src/IndexManager.cpp
        src/IndexManager.h

        )
