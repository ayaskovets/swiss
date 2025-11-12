cmake_minimum_required(VERSION 3.29.2 FATAL_ERROR)

project(swiss
    LANGUAGES CXX
    VERSION 0.0.1
)

set(TARGET swiss)
add_library(${TARGET}
    INTERFACE
)
target_include_directories(${TARGET}
    INTERFACE
    include
)

enable_testing()
add_subdirectory(tests/unit)
