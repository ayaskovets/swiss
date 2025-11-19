cmake_minimum_required(VERSION 3.29.2)
project(swiss VERSION 0.0.1 LANGUAGES CXX)

#-------------------------------------------------------------------------------
# general settings and options
#-------------------------------------------------------------------------------

include(GNUInstallDirs)
include(CMakeDependentOption)

cmake_dependent_option(SWISS_BUILD_TESTING "Build tests"
    ON "BUILD_TESTING" OFF)
cmake_dependent_option(SWISS_INSTALL "Generate target for install"
    ON "PROJECT_IS_TOP_LEVEL;NOT CMAKE_SKIP_INSTALL_RULES" OFF)

if(NOT DEFINED SWISS_INSTALL_CMAKEDIR)
    set(SWISS_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        CACHE STRING "Install path package-related CMake files")
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# sources
#-------------------------------------------------------------------------------

set(SOURCES
    include/${PROJECT_NAME}/conditionally_runtime.hpp
    include/${PROJECT_NAME}/dynamic_extent.hpp
    include/${PROJECT_NAME}/leak_detector_allocator.hpp
    include/${PROJECT_NAME}/mixins.hpp
    include/${PROJECT_NAME}/predicates.hpp
    include/${PROJECT_NAME}/scope_exit.hpp
    include/${PROJECT_NAME}/static_pimpl.hpp
    include/${PROJECT_NAME}/strong_typedef.hpp
    include/${PROJECT_NAME}/unchecked_any.hpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

#-------------------------------------------------------------------------------
# target
#-------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_compile_definitions(${PROJECT_NAME} INTERFACE "$<$<BOOL:${SWISS_BUILD_SHARED_LIBS}>:SWISS_SHARED_DEFINE>")
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

target_include_directories(${PROJECT_NAME}
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

if(SWISS_INSTALL)
    configure_package_config_file(cmake/${PROJECT_NAME}-config.cmake.in
        ${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION "${SWISS_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
        COMPATIBILITY AnyNewerVersion)

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_export
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE)

    set(TARGETS_FILE "${PROJECT_NAME}-targets.cmake")

    install(EXPORT ${PROJECT_NAME}_export
        FILE "${TARGETS_FILE}"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}"
        NAMESPACE ${PROJECT_NAME}::)
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION "${SWISS_INSTALL_CMAKEDIR}")
endif()

#-------------------------------------------------------------------------------
# etc.
#-------------------------------------------------------------------------------

if(SWISS_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests/unit)
endif()
