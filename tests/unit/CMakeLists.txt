set(TARGET tests)

find_package(GTest REQUIRED)

add_executable(${TARGET})
target_sources(${TARGET}
    PRIVATE
    conditionally_runtime_test.cpp
    leak_detector_allocator_test.cpp
    predicates_test.cpp
    static_pimpl_test.cpp
    strong_typedef_test.cpp
    unchecked_any_test.cpp
)
target_link_libraries(${TARGET}
    PRIVATE
    swiss
    GTest::gtest_main
)

gtest_discover_tests(${TARGET})
