cmake_minimum_required(VERSION 3.25.0)
project(
  swiss
  VERSION 0.0.1
  LANGUAGES CXX)

# ------------------------------------------------------------------------------
# general settings and options
# ------------------------------------------------------------------------------

add_library(swiss INTERFACE)
add_library(swiss::swiss ALIAS swiss)

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------

set(HEADERS
    include/swiss/conditionally_runtime.hpp
    include/swiss/dynamic_extent.hpp
    include/swiss/leak_detector_allocator.hpp
    include/swiss/mixins.hpp
    include/swiss/predicates.hpp
    include/swiss/scope_exit.hpp
    include/swiss/static_pimpl.hpp
    include/swiss/strong_typedef.hpp
    include/swiss/unchecked_any.hpp)

# ------------------------------------------------------------------------------
# target
# ------------------------------------------------------------------------------

target_sources(
  swiss INTERFACE FILE_SET HEADERS BASE_DIRS
                  "${CMAKE_CURRENT_SOURCE_DIR}/include" FILES ${HEADERS})

target_compile_features(swiss INTERFACE cxx_std_23)

# ------------------------------------------------------------------------------
# install
# ------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  configure_package_config_file(
    cmake/swiss-config.cmake.in swiss-config.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/swiss")

  write_basic_package_version_file(swiss-config-version.cmake
                                   COMPATIBILITY SameMajorVersion)

  install(
    TARGETS swiss
    EXPORT swiss_export
    FILE_SET HEADERS)

  install(
    EXPORT swiss_export
    FILE "swiss-targets.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/swiss"
    NAMESPACE swiss::)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/swiss-config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/swiss-config-version.cmake"
          DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/swiss")
endif()

# ------------------------------------------------------------------------------
# tests
# ------------------------------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)

  if(BUILD_TESTING)
    add_subdirectory(tests/unit)
  endif()
endif()
