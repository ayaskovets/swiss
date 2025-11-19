cmake_minimum_required(VERSION 3.29.2)
project(swiss-unit-tests)

#-------------------------------------------------------------------------------
# general settings and options
#-------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
endif()

#-------------------------------------------------------------------------------
# dependencies
#-------------------------------------------------------------------------------

find_package(GTest REQUIRED)

if(PROJECT_IS_TOP_LEVEL)
    find_package(swiss REQUIRED)
endif()

#-------------------------------------------------------------------------------
# sources
#-------------------------------------------------------------------------------

set(SOURCES
    conditionally_runtime_test.cpp
    leak_detector_allocator_test.cpp
    predicates_test.cpp
    static_pimpl_test.cpp
    strong_typedef_test.cpp
    unchecked_any_test.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

#-------------------------------------------------------------------------------
# target
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    swiss::swiss
    GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME})
