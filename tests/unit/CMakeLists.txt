cmake_minimum_required(VERSION 3.23)
project(swiss-unit-tests LANGUAGES CXX)

# ------------------------------------------------------------------------------
# general settings and options
# ------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
endif()

add_executable(swiss-unit-tests)
add_executable(swiss::unit-tests ALIAS swiss-unit-tests)

# ------------------------------------------------------------------------------
# dependencies
# ------------------------------------------------------------------------------

find_package(GTest REQUIRED)

if(PROJECT_IS_TOP_LEVEL)
  find_package(swiss REQUIRED)
endif()

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------

set(SOURCES
    conditionally_runtime_test.cpp
    leak_detector_allocator_test.cpp
    predicates_test.cpp
    scope_exit_test.cpp
    static_pimpl_test.cpp
    strong_typedef_test.cpp
    unchecked_any_test.cpp)

# ------------------------------------------------------------------------------
# target
# ------------------------------------------------------------------------------

target_sources(swiss-unit-tests PRIVATE ${SOURCES})
target_link_libraries(swiss-unit-tests PRIVATE swiss::swiss GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(swiss-unit-tests)
