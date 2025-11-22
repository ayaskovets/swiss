cmake_minimum_required(VERSION 3.29.2)
project(swiss-unit-tests)

#-------------------------------------------------------------------------------
# general settings and options
#-------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
endif()

add_executable(swiss-unit-tests)
add_executable(swiss::unit-tests ALIAS swiss-unit-tests)

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

#-------------------------------------------------------------------------------
# target
#-------------------------------------------------------------------------------

target_sources(swiss-unit-tests PRIVATE ${SOURCES})
target_link_libraries(swiss-unit-tests
    PRIVATE
    swiss::swiss
    GTest::gtest_main)

if(NOT PROJECT_IS_TOP_LEVEL)
    target_link_options(swiss-unit-tests PRIVATE ${SWISS_LINKER_FLAGS})
endif()

gtest_discover_tests(swiss-unit-tests)
