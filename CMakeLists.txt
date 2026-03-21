cmake_minimum_required(VERSION 3.23)
project(swiss VERSION 0.0.1 LANGUAGES CXX)

#-------------------------------------------------------------------------------
# general settings and options
#-------------------------------------------------------------------------------

include(GNUInstallDirs)

option(SWISS_BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(SWISS_INSTALL "Generate target for install" ${PROJECT_IS_TOP_LEVEL})

set(SWISS_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/cmake/swiss"
    CACHE STRING "Install path package-related CMake files")

add_library(swiss INTERFACE)
add_library(swiss::swiss ALIAS swiss)

#-------------------------------------------------------------------------------
# target
#-------------------------------------------------------------------------------

target_sources(swiss
    INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/swiss/conditionally_runtime.hpp
        include/swiss/dynamic_extent.hpp
        include/swiss/leak_detector_allocator.hpp
        include/swiss/mixins.hpp
        include/swiss/predicates.hpp
        include/swiss/scope_exit.hpp
        include/swiss/static_pimpl.hpp
        include/swiss/strong_typedef.hpp
        include/swiss/unchecked_any.hpp
)
target_compile_features(swiss INTERFACE cxx_std_23)
target_include_directories(swiss
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")

#-------------------------------------------------------------------------------
# install
#-------------------------------------------------------------------------------

if(SWISS_INSTALL)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(cmake/swiss-config.cmake.in
        swiss-config.cmake
        INSTALL_DESTINATION "${SWISS_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(swiss-config-version.cmake
        COMPATIBILITY AnyNewerVersion)

    install(TARGETS swiss
        EXPORT swiss_export
        FILE_SET HEADERS)

    install(EXPORT swiss_export
        FILE "swiss-targets.cmake"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}"
        NAMESPACE swiss::)
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/swiss-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/swiss-config-version.cmake"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}")
endif()

#-------------------------------------------------------------------------------
# tests
#-------------------------------------------------------------------------------

if(SWISS_BUILD_TESTING)
    include(CTest)
    add_subdirectory(tests/unit)
endif()
