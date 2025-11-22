cmake_minimum_required(VERSION 3.29.2)
project(swiss VERSION 0.0.1 LANGUAGES CXX)

#-------------------------------------------------------------------------------
# general settings and options
#-------------------------------------------------------------------------------

include(GNUInstallDirs)
include(CMakeDependentOption)

if(NOT DEFINED SWISS_BUILD_TESTING)
    if(DEFINED BUILD_TESTING AND BUILD_TESTING)
        set(SWISS_BUILD_TESTING ON CACHE BOOL "Build tests")
    else()
        set(SWISS_BUILD_TESTING OFF CACHE BOOL "Build tests")
    endif()
endif()

if(NOT DEFINED SWISS_INSTALL)
    if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_SKIP_INSTALL_RULES)
        set(SWISS_INSTALL ${BUILD_TESTING} CACHE BOOL "Generate target for install")
    else()
        set(SWISS_INSTALL OFF CACHE BOOL "Generate target for install")
    endif()
endif()

if(NOT DEFINED SWISS_CXX_FLAGS)
    set(SWISS_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()
string(REPLACE " " ";" SWISS_CXX_FLAGS "${SWISS_CXX_FLAGS}")

if(NOT DEFINED SWISS_LINKER_FLAGS)
    set(SWISS_LINKER_FLAGS ${CMAKE_STATIC_LINKER_FLAGS})
endif()
string(REPLACE " " ";" SWISS_LINKER_FLAGS "${SWISS_LINKER_FLAGS}")

if(NOT DEFINED SWISS_INSTALL_CMAKEDIR)
    set(SWISS_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/swiss"
        CACHE STRING "Install path package-related CMake files")
endif()

add_library(swiss INTERFACE)
add_library(swiss::swiss ALIAS swiss)

#-------------------------------------------------------------------------------
# sources
#-------------------------------------------------------------------------------

set(SOURCES
    include/swiss/conditionally_runtime.hpp
    include/swiss/dynamic_extent.hpp
    include/swiss/leak_detector_allocator.hpp
    include/swiss/mixins.hpp
    include/swiss/predicates.hpp
    include/swiss/scope_exit.hpp
    include/swiss/static_pimpl.hpp
    include/swiss/strong_typedef.hpp
    include/swiss/unchecked_any.hpp)

#-------------------------------------------------------------------------------
# target
#-------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(swiss PRIVATE ${SOURCES})
target_compile_features(swiss INTERFACE cxx_std_23)
target_compile_options(swiss INTERFACE ${SWISS_CXX_FLAGS})
target_include_directories(swiss
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

if(SWISS_INSTALL)
    configure_package_config_file(cmake/swiss-config.cmake.in
        swiss-config.cmake
        INSTALL_DESTINATION "${SWISS_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(swiss-config-version.cmake
        COMPATIBILITY AnyNewerVersion)

    install(TARGETS swiss
        EXPORT swiss_export
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE)

    set(TARGETS_FILE "swiss-targets.cmake")

    install(EXPORT swiss_export
        FILE "${TARGETS_FILE}"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}"
        NAMESPACE swiss::)
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/swiss-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/swiss-config-version.cmake"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}")
endif()

#-------------------------------------------------------------------------------
# etc.
#-------------------------------------------------------------------------------

if(SWISS_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests/unit)
endif()
